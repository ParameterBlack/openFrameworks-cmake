#pragma once

#include "ofCamera.h"
#include "ofEvents.h"


/// \brief A super simple camera for interacting with objects in 3D space.
class ofEasyCam : public ofCamera {
public:
    /// \name Constructor and Destructor
	/// \{
    
    /// \brief Create a default camera.
	ofEasyCam();

    /// \brief Destroy the camera.
	~ofEasyCam();

	/// \}
	/// \name Rendering
	/// \{

	virtual void begin(ofRectangle viewport = ofRectangle());

    /// \brief Reset the camera position and orientation.
	void reset();

	/// \}
	/// \name Camera Target
	/// \{

    /// \brief Set the camera's target.
    /// \param target The position of the target.
	void setTarget(const ofVec3f& target);

    /// \brief Set the camera's target.
    /// \param target The position of the target.
	void setTarget(ofNode& target);

    /// \brief Get the camera's target node reference.
    /// \returns a reference the the camera's target node.
	const ofNode& getTarget() const;

	/// \}
	/// \name Getters
	/// \{

    /// \brief Set the camera's distance to the target.
    /// \param distance The distance to the target.
	void setDistance(float distance);

    /// \brief Get the distance to the target.
    /// \returns the distance to the target.
	float getDistance() const;

    /// \brief Set the camera's drag coefficient.
    ///
    /// Drag is affects how quickly the camera starts up and slows down when
    /// a user interacts with its position using a pointer.
    ///
    /// \param drag The normalized coefficient value between 0 and 1.
	void setDrag(float drag);

    /// \brief Get the camera's drag coefficient.
    /// \returns the camera's drag coefficient.
	float getDrag() const;

	/// \}
	/// \name Setters
	/// \{

	/// \brief Enable or disable camera autodistance.
    ///
    /// Allow the camera to attempt to set the distance based on the camera's
    /// current viewpoirt.
    ///
    /// \param bAutoDistance true to enable auto distance.
    void setAutoDistance(bool bAutoDistance);

    void setEvents(ofCoreEvents & events);
	
	/// \brief Set the input sensitivity of the rotation.
	/// X and Y axes - when the value is 1.0, moving the mouse from one side to
	/// the other of the arcball (min(viewport.width, viewport.height)) will
	/// rotate 180 degrees. When the value is 0.5, the rotation will be 90
	/// degrees.
	/// \param value Scales the xyz axes rotation factor by these values.
	void setRotationSensitivity(float x, float y, float z);
    
    /// \brief Set the input sensitivity of the translation.
    /// \param value Scales the xyz axes translation factor by these values.
    void setTranslationSensitivity(float x, float y, float z);
		
    /// \brief Set the key used to switch between camera rotation and translation.
    ///
    /// Translation will only happen when the translation key is pressed.
    ///
    /// \param key The key code for the translation key.
    /// \todo char is not the right data type for this. Should be int.
	void setTranslationKey(char key);

    /// \brief Get the current translation key code.
    /// \returns the current translation key code.
    char getTranslationKey() const;

    /// \}
    /// \name Mouse Input
    /// \{

    /// \brief Enable mouse camera control.
	void enableMouseInput();

    /// \brief Disable mouse camera control.
	void disableMouseInput();

    /// \brief Determine if mouse camera control is enabled.
    /// \todo Rename to isMouseInputEnabled().
    /// \returns true iff mouse camera control is enabled.
	bool getMouseInputEnabled() const;

    /// \brief Enable the mouse's middle button for camera control.
	void enableMouseMiddleButton();

    /// \brief Disable the mouse's middle button for camera control.
	void disableMouseMiddleButton();

    /// \brief Determine if the middle mouse button is enabled.
    /// \todo Rename to isMouseMiddleButtonEnabled().
    /// \returns true iff the mouse's middle button is enabled.
	bool getMouseMiddleButtonEnabled() const;

	/// \}

	/// Uses Y axis relative to the camera orientation
	///
	/// By default the Y axis used for interactive rotation
	/// is vec3(0,1,0) or whatever is set as up axis using
	/// setUpAxis
	void setRelativeYAxis(bool relative=true);

	/// Set the camera fixed up axis for interactive
	/// manipulation
	void setUpAxis(const ofVec3f & up);

	void enableInertia();
	void disableInertia();
	
private:
	void setDistance(float distance, bool save);

	ofNode target;

	bool bEnableMouseMiddleButton = true;
	bool bApplyInertia = false;
	bool bDoTranslate = false;
	bool bDoRotate = false;
	bool bDoScrollZoom = false;
	bool bInsideArcball = false;
	bool bMouseInputEnabled = false;
	bool bDistanceSet = false;
	bool bAutoDistance = true;
	bool bEventsSet = false;
	float lastDistance = 0.f;

	float drag = 0.9f;
	
	float xRot = 0.0f;
	float yRot = 0.0f;
	float zRot = 0.0f;
	
	float moveX = 0.0f;
	float moveY = 0.0f;
	float moveZ = 0.0f;
	
	float sensitivityX = 1.0f;
	float sensitivityY = 1.0f;
	float sensitivityZ = 1.0f;
	float sensitivityRotX = 1.0f;
	float sensitivityRotY = 1.0f;
	float sensitivityRotZ = 1.0f;

	ofVec2f lastMouse, prevMouse;
	ofVec2f mouseVel;
	
	void updateRotation();
	void updateTranslation();
	void update(ofEventArgs & args);
	void mousePressed(ofMouseEventArgs & mouse);
	void mouseReleased(ofMouseEventArgs & mouse);
	void mouseDragged(ofMouseEventArgs & mouse);
	void mouseScrolled(ofMouseEventArgs & mouse);
	void updateMouse(const ofMouseEventArgs & mouse);
	ofVec3f up() const;

    /// \brief The key used to differentiate between translation and rotation.
	char doTranslationKey = 'm';

    /// \brief The time of the last pointer down event.
	unsigned long lastTap = 0;

    /// \brief The current rotation quaternion.
	ofQuaternion curRot;

    /// \brief The previous X axis.
    ofVec3f prevAxisX;

    /// \brief The previous Y axis.
    ofVec3f prevAxisY;

    /// \brief The previous Z axis.
    ofVec3f prevAxisZ;

    /// \brief the previous camera position.
	ofVec3f prevPosition;

    /// \brief The previous camera orientation.
	ofQuaternion prevOrientation;

	ofRectangle viewport;

	ofCoreEvents * events = nullptr;

	bool relativeYAxis = false;
	bool doInertia = false;
	ofVec3f upAxis{0,1,0};
};
