#include "WPILib.h"
#include <cmath>

class Robot: public IterativeRobot
{
private:
  LiveWindow *lw = LiveWindow::GetInstance();

	IMAQdxSession session;
	Image *frame;
	IMAQdxError imaqError;

	void RobotInit() {


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
		IMAQdxGrab(session, frame, true, NULL);
		CameraServer::GetInstance()->SetImage(frame);
  }

	void AutonomousInit()
	{

	}

	void AutonomousPeriodic() 
	{
		IMAQdxGrab(session, frame, true, NULL);
		CameraServer::GetInstance()->SetImage(frame);
	}

	void TeleopInit() 
	{

	}
	void TeleopPeriodic() 
	{
		IMAQdxGrab(session, frame, true, NULL);
		CameraServer::GetInstance()->SetImage(frame);

	}

	void TestPeriodic() {
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
