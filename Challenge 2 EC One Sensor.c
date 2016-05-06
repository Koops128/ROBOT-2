#pragma config(Sensor, S1,     colorLeft,      sensorEV3_Color)
#pragma config(Sensor, S3,     sonar,          sensorEV3_Ultrasonic)
#pragma config(Sensor, S4,     colorRight,     sensorEV3_Color)
#pragma config(Motor,  motorA,          motorLeft,     tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          motorRight,    tmotorEV3_Large, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
* TCSS 437 - Challenge 02 Extra Credit (Single sensor)
* Team #8: Aaron Chau, Abigail Smith, Sean Markus
* Base Code from: Team #12: Aaron Chau, Brandon Scholer
*/
#define MOTOR_SPEED_FAST 45
#define MOTOR_SPEED_NORMAL 30//35
#define MIN_DIR_INTERVAL 750
#define MAX_TURNS 2


int blackCal;
int whiteCal;
int grayCal;
int leftColorAvg;

/**
* follows the edge of a line
* param bool left: are we on the left side of a line?
*/
task followLine() {
    int runningSpeed = MOTOR_SPEED_NORMAL-5;
    clearTimer(T3);
    bool online = true;
    float offset = 3.0;
    float smallTurn = 0.3;
    float bigTurn = 0.5;
    while (online) {
	  	displayCenteredBigTextLine(7, "current: %d", leftColorAvg);

        // Go straight as long as the sensor is +-3 of the gray calibration.
        while (leftColorAvg <= grayCal + offset  && leftColorAvg >= grayCal - offset) {
            setMotorSpeed(motorLeft, runningSpeed);
            setMotorSpeed(motorRight, runningSpeed);
            setLEDColor(ledOff);
        }

        // Turn our robot to the right when our sensor is going towards the white (going off the line).
        while (leftColorAvg > grayCal + offset) {	// turn right
            // Do a forward biased right turn. 
            setMotorSpeed(motorLeft, runningSpeed);
            setMotorSpeed(motorRight, runningSpeed*smallTurn);
            // If the robot is just on white now, do a stationary right turn.
            while (leftColorAvg > whiteCal - offset && leftColorAvg > grayCal + offset) {
            	setMotorSpeed(motorLeft, runningSpeed*bigTurn);
            	setMotorSpeed(motorRight, -runningSpeed*bigTurn);
            	setLEDColor(ledGreen);
          	}
        }

        // Turn our robot to the left when our sensor is going towards the black line.
        while (leftColorAvg < grayCal - offset) {	// turn left
            // Do a forward biased left turn. 
            setMotorSpeed(motorLeft, runningSpeed*smallTurn);
            setMotorSpeed(motorRight, runningSpeed);
            // If the robot is on the black line, do a stationary left turn.
            while (leftColorAvg < blackCal + offset && leftColorAvg < grayCal - offset) {
            	setMotorSpeed(motorRight, runningSpeed*bigTurn);
            	setMotorSpeed(motorLeft, -runningSpeed*bigTurn);
            	setLEDColor(ledRed);
          	}
        }
    }
}


/**
* Uses a weighted moving average to keep track
* of color and sensor values
*/
task calculateAverages() {
    leftColorAvg = 0;
    int leftCount = 0;

    int i = 0;
    for (i = 0; i < 10; i ++) {
        leftColorAvg += getColorReflected(colorLeft);
    }
    leftColorAvg /= 10;

    float alpha = 0.5;
    while (1) {
        int currLeft = getColorReflected(colorLeft);

        if (leftColorAvg - currLeft < 20 || leftCount > 3) {
        	leftColorAvg =  currLeft + alpha * (leftColorAvg - currLeft);
        	leftCount = 0;
        } else {
        	leftCount++;
      	}
      	displayCenteredBigTextLine(11, "color avg = %d", leftColorAvg);
    }
}

/**
* The main function that contains the driver code.
*/
task main()
{
    whiteCal = 0;
    blackCal = 0;
    int sampleSize = 10;
    bool hasCalibrated = false;
    // ALways calculate the white first! 
    while (!hasCalibrated) {
        int i = 0;
        // Press the up button to calibrate the white average.
        if (getButtonPress(buttonUp) && !whiteCal) {
	        for (i = 0; i < sampleSize; i++) {
	        	sleep(100);
	        	whiteCal += getColorReflected(colorLeft);
	        }
	        whiteCal /= sampleSize;
	  			displayCenteredBigTextLine(1, "white calibrated");
        // Press the down button to calibrate the black average.
        } else if (getButtonPress(buttonDown) && !blackCal) {
            for (i = 0; i < sampleSize; i++) {
                sleep(100);
                blackCal += getColorReflected(colorLeft);
            }
            blackCal /= sampleSize;
            hasCalibrated = true;
            displayCenteredBigTextLine(1, "black calibrated");
        }
        setMotorSpeed(motorLeft, 0);
        setMotorSpeed(motorRight, 0);
    }

    // The gray calibration is the black + white / 2
    grayCal = (blackCal+whiteCal) / 2;

    // Display some values
    displayCenteredBigTextLine(1, "w = %d", whiteCal);
    displayCenteredBigTextLine(3, "b = %d", blackCal);
    displayCenteredBigTextLine(5, "g = %d", grayCal);

    startTask(calculateAverages);
    sleep(1000);
    startTask(followLine);

    while(1) {
        wait(10);
    }
}
