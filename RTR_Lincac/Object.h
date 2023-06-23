#pragma once

#ifndef _OBJECT_H
#define _OBJECT_H

#include<iostream>
#include"Help.h"
#include"AnimationData.h"
#include"AnimationHelp.h"
#include<map>

class Object {
public:
	virtual ~Object() {};
	virtual void render() = 0;
	virtual std::map<std::string, BoneInfo> GetBoneInfoMap() { return std::map<std::string,BoneInfo>(); }
	virtual int GetBoneCount() { return NULL; }
};

#endif // _OBJECT_H
