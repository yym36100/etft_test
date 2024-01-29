#pragma once
#include "obj.h"

namespace sg {

	class Events : public CObj {
	public:
		enum event{mmove,mpress,mrelease};
		event e;
		u16 x,y;

		Events(void){}
		~Events(void){}
	};


};// sg
