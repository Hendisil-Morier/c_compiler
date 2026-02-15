#ifndef ERROR_H
#define ERROR_H
	typedef struct
	{
		enum
		{
			SUCCESS_E,
			FAILURE_E,
			NOT_EXECUTE_E
		} state;
	} Status;
	//c dont allow comparing two different structs, so i made this helper
	static inline int status_isequal(const Status a, const Status b)
	{
		return a.state == b.state;
	}

	#define SUCCESS ((Status){SUCCESS_E})
	#define FAILURE ((Status){FAILURE_E})
	#define NOT_EXECUTE ((Status){NOT_EXECUTE_E})

#endif
