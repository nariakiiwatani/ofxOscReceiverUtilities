#pragma once

#include "ofxOscReceiver.h"

class ofxOscReceiverUtilities : public ofxOscReceiver
{
private:
	struct Value {
		enum {
			INT,FLOAT,STRING,
			VEC2,VEC3,VEC4,
			MAT3,MAT4,QUATERNION,
			CCOL,SCOL,FCOL,
			BUF,
			NONE
		} type;
		void *ptr;
		Value():type(NONE),ptr(NULL){}
		Value(int &val):type(INT),ptr(&val){}
		Value(float &val):type(FLOAT),ptr(&val){}
		Value(string &val):type(STRING),ptr(&val){}
		Value(ofVec2f &val):type(VEC2),ptr(&val){}
		Value(ofVec3f &val):type(VEC3),ptr(&val){}
		Value(ofVec4f &val):type(VEC4),ptr(&val){}
		Value(ofMatrix3x3 &val):type(MAT3),ptr(&val){}
		Value(ofMatrix4x4 &val):type(MAT4),ptr(&val){}
		Value(ofQuaternion &val):type(QUATERNION),ptr(&val){}
		Value(ofColor &val):type(CCOL),ptr(&val){}
		Value(ofShortColor &val):type(SCOL),ptr(&val){}
		Value(ofFloatColor &val):type(FCOL),ptr(&val){}
		Value(ofBuffer &val):type(BUF),ptr(&val){}
	};
	map<string, ofEvent<ofxOscMessage> > event_;
	multimap<string, Value> value_;
public:
	ofxOscReceiverUtilities() { ofAddListener(ofEvents().update, this, &ofxOscReceiverUtilities::update); }
	~ofxOscReceiverUtilities() { ofRemoveListener(ofEvents().update, this, &ofxOscReceiverUtilities::update); }
	template<class T>
	void addListener(const string &address, T *receiver, void (T::*method)(ofxOscMessage&)) {
		if(event_.find(address) == event_.end()) {
			event_.insert(make_pair(address, ofEvent<ofxOscMessage>()));
		}
		ofAddListener(event_[address], receiver, method);
	}
	template<class T>
	void removeListener(const string &address, T *receiver, void (T::*method)(ofxOscMessage&)) {
		if(event_.find(address) != event_.end()) {
			ofRemoveListener(event_[address], receiver, method);
		}
	}
	template<typename T>
	void addReference(const string &address, T &value) {
		value_.insert(make_pair(address, Value(value)));
	}
	template<typename T>
	void removeReference(const string &address, T &value) {
		for(multimap<string, Value>::iterator it = value_.lower_bound(address); it != value_.end() && it->first == address;) {
			if(it->second.ptr == &value) {
				value_.erase(it++);
			}
			else {
				++it;
			}
		}
	}
private:
	void update(ofEventArgs &arg) {
		while(hasWaitingMessages()) {
			ofxOscMessage msg;
			getNextMessage(&msg);
			const string &address = msg.getAddress();
			if(event_.find(address) != event_.end()) {
				ofNotifyEvent(event_[address], msg);
			}
			for(multimap<string, Value>::iterator it = value_.lower_bound(address); it != value_.end() && it->first == address; ++it) {
				setValue(msg, it->second);
			}
		}
	}
	void setValue(ofxOscMessage &msg, Value &value) {
		switch(value.type) {
			case Value::NONE:
				break;
			case Value::INT:
				*(int*)value.ptr = msg.getArgAsInt32(0);
				break;
			case Value::FLOAT:
				*(float*)value.ptr = msg.getArgAsFloat(0);
				break;
			case Value::STRING:
				*(string*)value.ptr = msg.getArgAsString(0);
				break;
			case Value::VEC2:
				((ofVec2f*)value.ptr)->set(msg.getArgAsFloat(0), msg.getArgAsFloat(1));
				break;
			case Value::VEC3:
				((ofVec3f*)value.ptr)->set(msg.getArgAsFloat(0), msg.getArgAsFloat(1), msg.getArgAsFloat(2));
				break;
			case Value::VEC4:
				((ofVec4f*)value.ptr)->set(msg.getArgAsFloat(0), msg.getArgAsFloat(1), msg.getArgAsFloat(2), msg.getArgAsFloat(3));
				break;
			case Value::MAT3:
				((ofMatrix3x3*)value.ptr)->set(msg.getArgAsFloat(0), msg.getArgAsFloat(1), msg.getArgAsFloat(2),
											   msg.getArgAsFloat(3), msg.getArgAsFloat(4), msg.getArgAsFloat(5),
											   msg.getArgAsFloat(6), msg.getArgAsFloat(7), msg.getArgAsFloat(8));
				break;
			case Value::MAT4:
				((ofMatrix4x4*)value.ptr)->set(msg.getArgAsFloat(0), msg.getArgAsFloat(1), msg.getArgAsFloat(2), msg.getArgAsFloat(3),
											   msg.getArgAsFloat(4), msg.getArgAsFloat(5), msg.getArgAsFloat(6), msg.getArgAsFloat(7),
											   msg.getArgAsFloat(8), msg.getArgAsFloat(9), msg.getArgAsFloat(10), msg.getArgAsFloat(11),
											   msg.getArgAsFloat(12), msg.getArgAsFloat(13), msg.getArgAsFloat(14), msg.getArgAsFloat(15));
				break;
			case Value::QUATERNION:
				((ofQuaternion*)value.ptr)->set(msg.getArgAsFloat(0), msg.getArgAsFloat(1), msg.getArgAsFloat(2), msg.getArgAsFloat(3));
				break;
			case Value::CCOL:
				((ofColor*)value.ptr)->set(msg.getArgAsInt32(0), msg.getArgAsInt32(1), msg.getArgAsInt32(2), msg.getArgAsInt32(3));
				break;
			case Value::SCOL:
				((ofShortColor*)value.ptr)->set(msg.getArgAsInt32(0), msg.getArgAsInt32(1), msg.getArgAsInt32(2), msg.getArgAsInt32(3));
				break;
			case Value::FCOL:
				((ofFloatColor*)value.ptr)->set(msg.getArgAsFloat(0), msg.getArgAsFloat(1), msg.getArgAsFloat(2), msg.getArgAsFloat(3));
				break;
			case Value::BUF:
				((ofBuffer*)value.ptr)->set(msg.getArgAsBlob(0));
				break;
		}
	}
};
