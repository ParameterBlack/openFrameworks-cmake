#include "ofEasyCam.h"
#include "ofMath.h"
#include "ofUtils.h"

// when an ofEasyCam is moving due to momentum, this keeps it
// from moving forever by assuming small values are zero.
static const float minDifference = 0.1e-5f;

// this is the default on windows os
static const unsigned long doubleclickTime = 200;

//----------------------------------------
ofEasyCam::ofEasyCam(){
	reset();
}

//----------------------------------------
ofEasyCam::~ofEasyCam(){
	disableMouseInput();
}

//----------------------------------------
void ofEasyCam::update(ofEventArgs & args){
	viewport = getViewport(this->viewport);
	if(!bDistanceSet && bAutoDistance){
		setDistance(getImagePlaneDistance(viewport), true);
	}
	if(bMouseInputEnabled){

		if(events->getMousePressed()) prevMouse = ofVec2f(events->getMouseX(),events->getMouseY());

		if (bDoRotate) {
			updateRotation();
		}else if (bDoTranslate || bDoScrollZoom) {
			updateTranslation(); 
			bDoScrollZoom = false;
		}
	}	
}

//----------------------------------------
void ofEasyCam::begin(ofRectangle _viewport){
	if(!bEventsSet){
		setEvents(ofEvents());
	}
	viewport = getViewport(_viewport);
	ofCamera::begin(viewport);
}

//----------------------------------------
void ofEasyCam::reset(){
	target.resetTransform();

	target.setPosition(0, 0, 0);
	lookAt(target);

	resetTransform();
	setPosition(0, 0, lastDistance);

	xRot = 0;
	yRot = 0;
	zRot = 0;

	moveX = 0;
	moveY = 0;
	moveZ = 0;

	bApplyInertia = false;
	bDoTranslate = false;
	bDoRotate = false;
}

//----------------------------------------
void ofEasyCam::setTarget(const ofVec3f& targetPoint){
	target.setPosition(targetPoint);
	lookAt(target);
}

//----------------------------------------
void ofEasyCam::setTarget(ofNode& targetNode){
	target = targetNode;
	lookAt(target);
}

//----------------------------------------
const ofNode& ofEasyCam::getTarget() const{
	return target;
}

//----------------------------------------
void ofEasyCam::setDistance(float distance){
	setDistance(distance, true);
}

//----------------------------------------
void ofEasyCam::setDistance(float distance, bool save){//should this be the distance from the camera to the target?
	if(distance > 0.0f){
		if(save){
			this->lastDistance = distance;
		}
		setPosition(target.getPosition() + (distance * getZAxis()));
		bDistanceSet = true;
	}
}

//----------------------------------------
float ofEasyCam::getDistance() const{
	return target.getPosition().distance(getPosition());
}

//----------------------------------------
void ofEasyCam::setAutoDistance(bool bAutoDistance){
	this->bAutoDistance = bAutoDistance;
	if(bAutoDistance){
		bDistanceSet = false;
	}
}

//----------------------------------------
void ofEasyCam::setDrag(float drag){
	this->drag = drag;
}

//----------------------------------------
float ofEasyCam::getDrag() const{
	return drag;
}

//----------------------------------------
void ofEasyCam::setTranslationKey(char key){
	doTranslationKey = key;
}

//----------------------------------------
char ofEasyCam::getTranslationKey() const{
	return doTranslationKey;
}

//----------------------------------------
void ofEasyCam::enableMouseInput(){
	if(!bMouseInputEnabled && events){
		ofAddListener(events->update, this, &ofEasyCam::update);
		ofAddListener(events->mouseDragged , this, &ofEasyCam::mouseDragged);
		ofAddListener(events->mousePressed, this, &ofEasyCam::mousePressed);
		ofAddListener(events->mouseReleased, this, &ofEasyCam::mouseReleased);
		ofAddListener(events->mouseScrolled, this, &ofEasyCam::mouseScrolled);
	}
	// if enableMouseInput was called within ofApp::setup()
	// `events` will still carry a null pointer, and bad things
	// will happen. Therefore we only update the flag. 
	bMouseInputEnabled = true;
	// setEvents() is called upon first load, and will make sure 
	// to enable the mouse input once the camera is fully loaded.
}

//----------------------------------------
void ofEasyCam::disableMouseInput(){
	if(bMouseInputEnabled && events){
		ofRemoveListener(events->update, this, &ofEasyCam::update);
		ofRemoveListener(events->mouseDragged, this, &ofEasyCam::mouseDragged);
		ofRemoveListener(events->mousePressed, this, &ofEasyCam::mousePressed);
		ofRemoveListener(events->mouseReleased, this, &ofEasyCam::mouseReleased);
		ofRemoveListener(events->mouseScrolled, this, &ofEasyCam::mouseScrolled);
	}
	// if disableMouseInput was called within ofApp::setup()
	// `events` will still carry a null pointer, and bad things
	// will happen. Therefore we only update the flag. 
	bMouseInputEnabled = false;
	// setEvents() is called upon first load, and will make sure 
	// to enable the mouse input once the camera is fully loaded.
}

//----------------------------------------
void ofEasyCam::setEvents(ofCoreEvents & _events){
	// If en/disableMouseInput were called within ofApp::setup(),
	// bMouseInputEnabled will tell us about whether the camera
	// mouse input needs to be initialised as enabled or disabled.
	// we will still set `events`, so that subsequent enabling
	// and disabling can work.

	// we need a temporary copy of bMouseInputEnabled, since it will 
	// get changed by disableMouseInput as a side-effect.
	bool wasMouseInputEnabled = bMouseInputEnabled || !events;
	disableMouseInput();
	events = &_events;
	if (wasMouseInputEnabled) {
		// note: this will set bMouseInputEnabled to true as a side-effect.
		enableMouseInput();
	}
	bEventsSet = true;
}

//----------------------------------------
void ofEasyCam::setRotationSensitivity(float x, float y, float z){
    sensitivityRotX = x;
    sensitivityRotY = y;
    sensitivityRotZ = z;
}

//----------------------------------------
void ofEasyCam::setTranslationSensitivity(float x, float y, float z){
    sensitivityX = x;
    sensitivityY = y;
    sensitivityZ = z;
}

//----------------------------------------
bool ofEasyCam::getMouseInputEnabled() const{
	return bMouseInputEnabled;
}

//----------------------------------------
void ofEasyCam::enableMouseMiddleButton(){
	bEnableMouseMiddleButton = true;
}

//----------------------------------------
void ofEasyCam::disableMouseMiddleButton(){
	bEnableMouseMiddleButton = false;
}

//----------------------------------------
bool ofEasyCam::getMouseMiddleButtonEnabled() const{
	return bEnableMouseMiddleButton;
}

//----------------------------------------
ofVec3f ofEasyCam::up() const{
	if(relativeYAxis){
		if(bApplyInertia){
			return getYAxis();
		}else{
			return prevAxisY;
		}
	}else{
		return upAxis;
	}
}

//----------------------------------------
void ofEasyCam::setRelativeYAxis(bool relative){
	relativeYAxis = relative;
}

//----------------------------------------
void ofEasyCam::setUpAxis(const ofVec3f & _up){
	upAxis = _up;
}

//----------------------------------------
void ofEasyCam::enableInertia(){
	doInertia = true;
}

//----------------------------------------
void ofEasyCam::disableInertia(){
	doInertia = false;
}

//----------------------------------------
void ofEasyCam::updateTranslation(){
	if(bApplyInertia){
		moveX *= drag;
		moveY *= drag;
		moveZ *= drag;
		if(ABS(moveX) <= minDifference && ABS(moveY) <= minDifference && ABS(moveZ) <= minDifference){
			bApplyInertia = false;
			bDoTranslate = false;
		}
		move((getXAxis() * moveX) + (getYAxis() * moveY) + (getZAxis() * moveZ));
	}else{
		setPosition(prevPosition + ofVec3f(prevAxisX * moveX) + (prevAxisY * moveY) + (prevAxisZ * moveZ));
	}
}	

//----------------------------------------
void ofEasyCam::updateRotation(){
	if(bApplyInertia){
		xRot *=drag; 
		yRot *=drag;
		zRot *=drag;

		if(ABS(xRot) <= minDifference && ABS(yRot) <= minDifference && ABS(zRot) <= minDifference){
			xRot = 0;
			yRot = 0;
			zRot = 0;
			bApplyInertia = false;
			bDoRotate = false;
		}
		curRot = ofQuaternion(xRot, getXAxis(), yRot, up(), zRot, getZAxis());
		setPosition((getGlobalPosition()-target.getGlobalPosition())*curRot +target.getGlobalPosition());
		rotate(curRot);
	}else{
		curRot = ofQuaternion(xRot, prevAxisX, yRot, up(), zRot, prevAxisZ);
		setPosition((prevPosition-target.getGlobalPosition())*curRot +target.getGlobalPosition());
		setOrientation(prevOrientation * curRot);
	}
}

//----------------------------------------
void ofEasyCam::mousePressed(ofMouseEventArgs & mouse){
	ofRectangle viewport = getViewport(this->viewport);
	if(viewport.inside(mouse.x, mouse.y)){
		lastMouse = mouse;
		prevMouse = mouse;
		prevAxisX = getXAxis();
		prevAxisY = getYAxis();
		prevAxisZ = getZAxis();
		prevPosition = ofCamera::getGlobalPosition();
		prevOrientation = ofCamera::getGlobalOrientation();

		if((bEnableMouseMiddleButton && mouse.button == OF_MOUSE_BUTTON_MIDDLE) || events->getKeyPressed(doTranslationKey)  || mouse.button == OF_MOUSE_BUTTON_RIGHT){
			bDoTranslate = true;
			bDoRotate = false;
		}else if(mouse.button == OF_MOUSE_BUTTON_LEFT){
			bDoTranslate = false;
			bDoRotate = true;
			if(ofVec2f(mouse.x - viewport.x - (viewport.width/2), mouse.y - viewport.y - (viewport.height/2)).length() < min(viewport.width/2, viewport.height/2)){
				bInsideArcball = true;
			}else{
				bInsideArcball = false;
			}
		}
		bApplyInertia = false;
	}
}

//----------------------------------------
void ofEasyCam::mouseReleased(ofMouseEventArgs & mouse){
	if(doInertia){
		unsigned long curTap = ofGetElapsedTimeMillis();
		ofRectangle viewport = getViewport(this->viewport);
		if(lastTap != 0 && curTap - lastTap < doubleclickTime){
			reset();
			return;
		}
		lastTap = curTap;
		bApplyInertia = true;
		mouseVel = mouse  - prevMouse;

		updateMouse(mouse);
		ofVec2f center(viewport.width/2, viewport.height/2);
		int vFlip;
		if(isVFlipped()){
			vFlip = -1;
		}else{
			vFlip =  1;
		}
		zRot = -vFlip * ofVec2f(mouse.x - viewport.x - center.x, mouse.y - viewport.y - center.y).angle(prevMouse - ofVec2f(viewport.x, viewport.y) - center);
	}else{
		bDoRotate = false;
		xRot = 0;
		yRot = 0;
		zRot = 0;

		bDoTranslate = false;
		moveX = 0;
		moveY = 0;
		moveZ = 0;
	}
}

//----------------------------------------
void ofEasyCam::mouseDragged(ofMouseEventArgs & mouse){
	mouseVel = mouse  - lastMouse;

	updateMouse(mouse);
}

//----------------------------------------
void ofEasyCam::mouseScrolled(ofMouseEventArgs & mouse){
	ofRectangle viewport = getViewport(this->viewport);
	prevPosition = ofCamera::getGlobalPosition();
	prevAxisZ = getZAxis();
	moveZ = mouse.scrollY * 30 * sensitivityZ * (getDistance() + FLT_EPSILON)/ viewport.height;
	bDoScrollZoom = true;
}

//----------------------------------------
void ofEasyCam::updateMouse(const ofMouseEventArgs & mouse){
	ofRectangle viewport = getViewport(this->viewport);
	int vFlip;
	if(isVFlipped()){
		vFlip = -1;
	}else{
		vFlip =  1;
	}
	if(bDoTranslate){
		moveX = 0;
		moveY = 0;
		moveZ = 0;
		if(mouse.button == OF_MOUSE_BUTTON_RIGHT){
			moveZ = mouseVel.y * (sensitivityZ * 0.7f) * (getDistance() + FLT_EPSILON)/ viewport.height;
		}else{
			moveX = -mouseVel.x * (sensitivityX * 0.5f) * (getDistance() + FLT_EPSILON)/viewport.width;
			moveY = vFlip * mouseVel.y * (sensitivityY* 0.5f) * (getDistance() + FLT_EPSILON)/viewport.height;
		}
	}else if(bDoRotate){
		xRot = 0;
		yRot = 0;
		zRot = 0;
		if(bInsideArcball){
			xRot = vFlip * -mouseVel.y * sensitivityRotX * 180 / min(viewport.width, viewport.height);
			yRot = -mouseVel.x * sensitivityRotY * 180 / min(viewport.width, viewport.height);
		}else{
			ofVec2f center(viewport.width/2, viewport.height/2);
			zRot = -vFlip * ofVec2f(mouse.x - viewport.x - center.x, mouse.y - viewport.y - center.y).angle(lastMouse - ofVec2f(viewport.x, viewport.y) - center) * sensitivityRotZ;
		}
	}
}
