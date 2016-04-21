#pragma config(Sensor, S1,     touchLeft,      sensorEV3_Touch)
#pragma config(Sensor, S4,     touchRight,     sensorEV3_Touch)
#pragma config(Motor,  motorB,          motorLeft,     tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          motorRight,    tmotorEV3_Large, PIDControl, encoder)

/*
* TCSS 437 - Challenge 01
* Team #12: Aaron Chau, Brandon Scholer
*/

#define MOTOR_SPEED_FAST 50
#define MOTOR_SPEED_NORMAL 37
#define MIN_DIR_INTERVAL 750
#define MAX_TURNS 2

// Function Prototype (Go to the function for more details.)
void leftBumperHit(int * motorLeftSpeed, int * motorRightSpeed);
void rightBumperHit(int * motorLeftSpeed, int * motorRightSpeed);
void bothBumpersHit(int * motorLeftSpeed, int * motorRightSpeed);
void moveLeft(int * motorLeftSpeed, int * motorRightSpeed);
void moveRight(int * motorLeftSpeed, int * motorRightSpeed);
void reverseRobot(int timeToReverse);
void setMyTimer(int * theTimer, int millis);
int resetMyTimer();

task wander() {
		// The motor speed
    int motorLeftSpeed, motorRightSpeed;

    // The time when the robot will change directions
    int timeToReset = resetMyTimer();

    // Choose a random direction to move.
    if(random(1))
        {
        		//set the motor speed values to turn left
            moveLeft(&motorLeftSpeed, &motorRightSpeed);
        }
        else
        {
        		//set the motor speed values to turn right
            moveRight(&motorLeftSpeed, &motorRightSpeed);
        }

    //Have a counter for left/right movement. Negative for left and pos for right.
    int moveCounter = 0;
    while(1) {
        // Get the time in milli when the left/right bumpers are hit.

            // Give motor left/right a random speed
            // Make robot walk forward.
            setMotorSpeed(motorLeft, motorLeftSpeed);
            setMotorSpeed(motorRight, motorRightSpeed);

        // If it is time, reset the direction
        if (time1[T2] > timeToReset)
        {
            // Get a new time.
            timeToReset = resetMyTimer();

            // Choose to go left or right.
            int leftRightRand = random(1);

            // If the robot has moved left/right too many times, force
            // it to go the other direction.
            if (moveCounter <= -1 * MAX_TURNS)
            {
                // move right
                leftRightRand = 0;
            }
            else if (moveCounter >= MAX_TURNS)
            {
                // move left
                leftRightRand = 1;
            }

            if(leftRightRand)
            {
                // Move left
                moveLeft(&motorLeftSpeed, &motorRightSpeed);
                if (moveCounter > 0) moveCounter = 0;
                moveCounter--;
            }
            else
            {
                // Move right
                moveRight(&motorLeftSpeed, &motorRightSpeed);
                if (moveCounter < 0) moveCounter = 0;
                moveCounter++;
            }
        }

        //Loop to monitor value in Sensor debugger window
        sleep(50);

    }
}

/**
* The main function that contains the driver code.
*/
task main()
{
    startTask(wander);

    while(1) {

  	}
}

/**
* Move the robot to the left.
*/
void moveLeft(int * motorLeftSpeed, int * motorRightSpeed) {
    *motorLeftSpeed = MOTOR_SPEED_NORMAL;
    *motorRightSpeed = MOTOR_SPEED_FAST;
}

/**
* Move the robot to the right.
*/
void moveRight(int * motorLeftSpeed, int * motorRightSpeed) {
    *motorLeftSpeed = MOTOR_SPEED_FAST;
    *motorRightSpeed = MOTOR_SPEED_NORMAL;
}

/**
* Set the timer at a certain millisecond.
*/
void setMyTimer(int * theTimer, int millis)
{
    clearTimer(T2);
    *theTimer = millis;
}

/**
* Return a random time interval between (MIN_DIR_INTERVAL ~ MIN_DIR_INTERVAL+100)
*/
int resetMyTimer() {
    clearTimer(T2);
    return MIN_DIR_INTERVAL + random(100);
}

/**
* Reverse the robot given a duration in milliseconds.
*/
void reverseRobot(int timeToReverse) {
    clearTimer(T1);
    while(time1[T1] < timeToReverse) {
        setMotorSpeed(motorLeft, -1 * MOTOR_SPEED_NORMAL);
        setMotorSpeed(motorRight, -1 * MOTOR_SPEED_NORMAL);
    }
}
