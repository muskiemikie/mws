/*
 * Sample.h
 *
 *  Created on: Dec 8, 2017
 *      Author: michaels
 */

#ifndef SAMPLE_H_
#define SAMPLE_H_

#include "Common.h"
#include "Engine.h"


class Sample: public mws::Engine {
public:
	Sample();
	virtual ~Sample();

	virtual void Initialize() override;

protected:
	virtual void Update() override;
	virtual void Render() override;

	virtual bool MessageHandler(unsigned uMsg, WPARAM wParam, LPARAM lParam) override;
};


#endif /* SAMPLE_H_ */
