// CHANGES
// Converted to C++
// Better Vision Processing (Use in tandem with RoboRealm or OpenCV)
// Added Data to Dashboard
// Considering using "Fake" system

//Check for pointer/other syntactic errors

/*
Experiments:
https://www.chiefdelphi.com/forums/showthread.php?t=143939
https://wpilib.screenstepslive.com/s/4485/m/50711/l/479908-reading-array-values-published-by-networktables
https://github.com/WPIRoboticsProjects/GRIP/wiki/Tutorial:-Run-GRIP-from-a-CPP,-Java,-or-LabVIEW-FRC-program
http://pastebin.com/WNrj4aUe
https://www.chiefdelphi.com/forums/showthread.php?t=141283
*/

#include "WPILib.h"
#include <cmath>

class Robot: public IterativeRobot
{
private:
  LiveWindow *lw = LiveWindow::GetInstance();
	
	Joystick *stick1 = new Joystick(0);
	Joystick *stick2 = new Joystick(1);
	Joystick *stick3 = new Joystick(2);
	
//Fake Land
//
	TalonSRX *Fake_rearRight = new TalonSRX(1);
	TalonSRX *Fake_rearLeft = new TalonSRX(2);
	TalonSRX *Fake_frontLeft = new TalonSRX(3);
	TalonSRX *Fake_frontRight = new TalonSRX(4);
//

	CANTalon *rearRight = new CANTalon(41);
	CANTalon *rearLeft = new CANTalon(42);
	CANTalon *frontLeft = new CANTalon(43);
	CANTalon *frontRight = new CANTalon(44);
	
	CANJaguar *sus1 = new CANJaguar(25);
	CANJaguar *sus2 = new CANJaguar(26);
	CANJaguar *sus3 = new CANJaguar(27);
	CANJaguar *sus4 = new CANJaguar(28);
	
	
	rearRight->SetInverted(true);
	rearLeft->SetInverted(true);
  	frontLeft->SetInverted(true);
	frontRight->SetInverted(true);
	
	RobotDrive *Kinetic = new RobotDrive(Fake_frontLeft, Fake_rearLeft, Fake_frontRight, Fake_rearRight);
	
// Just because.	
//See https://wpilib.screenstepslive.com/s/4485/m/13809/l/230372-jaguar-speed-controllers (or other docs c++)
//"Using Position Control" (or other c++ docs)
	sus1->SetPositionMode(CANJaguar::kPotentiometer, -2000, -0, -0.0);
	sus2->SetPositionMode(CANJaguar::kPotentiometer, -2000, -0, -0.0);
	sus3->SetPositionMode(CANJaguar::kPotentiometer, 2000, 0, 0);
	sus4->SetPositionMode(CANJaguar::kPotentiometer, -2000, -0, -0);
	
	sus1->EnableControl();
	sus2->EnableControl();
	sus3->EnableControl();
	sus4->EnableControl();
// SetFeedbackDevice(arg) for the above?
	CANJaguar *shooter = new CANJaguar(30);
	CANJaguar *arm = new CANJaguar(31);
	Talon *intake = new Talon(0);

	IMAQdxSession session;
	Image *frame;
	IMAQdxError imaqError;

	void RobotInit() 
	{
		frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
		imaqError = IMAQdxOpenCamera("cam1", IMAQdxCameraControlModeController, &session);
		if(imaqError != IMAQdxErrorSuccess) {
			DriverStation::ReportError("IMAQdxOpenCamera error: " + std::to_string((long)imaqError) + "\n");
		}
		imaqError = IMAQdxConfigureGrab(session);
		if(imaqError != IMAQdxErrorSuccess) {
			DriverStation::ReportError("IMAQdxConfigureGrab error: " + std::to_string((long)imaqError) + "\n");
		}
		IMAQdxStartAcquisition(session);

	}

	void DisabledPeriodic() override
	{ 
//		Check; unnecessary?
//		(Scheduler::GetInstance())::run();
		IMAQdxGrab(session, frame, true, NULL);
		CameraServer::GetInstance()->SetImage(frame);
		
// Dashboard needed? List:
//		SmartDashboard::PutNumber(blah,blah2);
  	}


//	Add own autonomous code.
	void AutonomousInit()
	{

	}

	void AutonomousPeriodic() 
	{
//		Check; unnecessary
//		(Scheduler::GetInstance())::run();
		IMAQdxGrab(session, frame, true, NULL);
		CameraServer::GetInstance()->SetImage(frame);


//		If it matters...		
		SmartDashboard::PutNumber("rearLeft Volts", (rearLeft->GetOutputVoltage()));
		SmartDashboard::PutNumber("rearRight Volts", (rearRight->GetOutputVoltage()));
		SmartDashboard::PutNumber("frontLeft Volts", (frontLeft->GetOutputVoltage()));
		SmartDashboard::PutNumber("frontRight Volts", (frontRight->GetOutputVoltage()));
//		Other values not needed shooter, arm, intake
		SmartDashboard::PutNumber("shooter Volts", (shooter->GetOutputVoltage()));
		SmartDashboard::PutNumber("arm Volts", (arm->GetOutputVoltage()));
		SmartDashboard::PutNumber("intake Volts", (intake->GetOutputVoltage()));
//		Suspension Dashboard		
		SmartDashboard::PutNumber("sus1 Position", (sus1->GetPosition()));
		SmartDashboard::PutNumber("sus2 Position", (sus2->GetPosition()));
		SmartDashboard::PutNumber("sus3 Position", (sus3->GetPosition()));
		SmartDashboard::PutNumber("sus4 Position", (sus4->GetPosition()));
		
//http://first.wpi.edu/FRC/roborio/stable/docs/cpp/classRobotDrive.html#a00f3c6f7906685fe2f98ceaa8bb7e188
//		Consider using RobotDrive->Drive(arg1, arg2);



//For actual code, change control mode to position?
	}

	void TeleopInit() 
	{
		frontLeft->SetControlMode(CANTalon::kPercentVbus);
		rearLeft->SetControlMode(CANTalon::kPercentVbus);
		frontRight->SetControlMode(CANTalon::kPercentVbus);
		rearRight->SetControlMode(CANTalon::kPercentVbus);

	}
	void TeleopPeriodic() 
	{
		IMAQdxGrab(session, frame, true, NULL);
		CameraServer::GetInstance()->SetImage(frame);

		SmartDashboard::PutNumber("rearLeft Volts", (rearLeft->GetOutputVoltage()));
		SmartDashboard::PutNumber("rearRight Volts", (rearRight->GetOutputVoltage()));
		SmartDashboard::PutNumber("frontLeft Volts", (frontLeft->GetOutputVoltage()));
		SmartDashboard::PutNumber("frontRight Volts", (frontRight->GetOutputVoltage()));
		
		SmartDashboard::PutNumber("shooter Volts", (shooter->GetOutputVoltage()));
		SmartDashboard::PutNumber("arm Volts", (arm->GetOutputVoltage()));
		SmartDashboard::PutNumber("intake Volts", (intake->GetOutputVoltage()));
//		Suspension Dashboard		
		SmartDashboard::PutNumber("sus1 Position", (sus1->GetPosition()));
		SmartDashboard::PutNumber("sus2 Position", (sus2->GetPosition()));
		SmartDashboard::PutNumber("sus3 Position", (sus3->GetPosition()));
		SmartDashboard::PutNumber("sus4 Position", (sus4->GetPosition()));


//		TankDrive? Fakes?
//		second arg not needed?
		Kinetic->ArcadeDrive(stick1, false);
		frontLeft->Set(Fake_frontLeft->Get());
		rearLeft->Set(Fake_rearLeft->Get());
		frontRight->Set(Fake_frontRight->Get());
		rearRight->Set(Fake_rearRight->Get());
	
//		Precalculated
//		based of potentiometer table
		sus1->Set(stick2->getY() * 0.060 + .497);
		sus2->Set(stick2->getY() * 0.060 + .349);
		sus3->Set(stick2->getY() * -0.060 + 0.625);
		sus4->Set(stick2->getY() * 0.060 + 0.278);

		shooter->Set(stick2->GetThrottle());
		arm->Set(stick3->getRawAxis(1));
		intake->Set(stick1->getRawAxis(2));

	}

	void TestPeriodic() {
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
