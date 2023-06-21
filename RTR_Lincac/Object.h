#pragma once

#ifndef _OBJECT_H
#define _OBJECT_H

#include<iostream>
#include"Light.h"

class Object {
public:
	virtual ~Object() {};
	virtual void render() = 0;
};

#endif // _OBJECT_H
