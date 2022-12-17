#ifndef CTL_EXCEPTION_HPP
#define CTL_EXCEPTION_HPP


#include <string>


#define CTL_NEED_ABORT true
#define CTL_NOT_NEED_ABORT false


namespace ctl { /* 'ctl' = custom template library */


class exception final
{

  private:

	std::string _what;  // The reason for the exception
	std::string _where; // From where the exception was thrown
	bool _need_abort;   // Marks if programm was crached and needs abort/terminate/...

  public:

	exception(void) = delete;
	explicit exception(const std::string & what,
								const std::string & where = "", bool need_abort = CTL_NEED_ABORT) :
        _what(what), _where(where), _need_abort(need_abort)
    { }

	~exception() noexcept { }

	std::string what(void) const noexcept
    { return _what; }

	std::string where(void) const noexcept
    { return _where; }

	bool need_abort(void) const noexcept
    { return _need_abort; }

	std::string error_message(void) const noexcept
    { return "'" + _what + "' in '" + _where + "'. " + (_need_abort ? "Crashed" : "No crash"); }

};


} /* namespace 'ctl' */


#endif /* CTL_EXCEPTION_HPP */

