#ifndef CUSTOM_EXCEPTION_HPP
#define CUSTOM_EXCEPTION_HPP


#include <string>


#define NEED_ABORT true
#define NOT_NEED_ABORT false


class custom_exception final
{

	private:

		std::string _what;
		std::string _where;
		bool _need_abort;

	public:

		custom_exception(void) = delete;
		explicit custom_exception(const std::string & what, 
								  const std::string & where = "", bool need_abort = true) :
        _what(what), _where(where), _need_abort(need_abort)
        { }
		
		~custom_exception() noexcept { }

		std::string what(void) const noexcept
        { return _what; }

		std::string where(void) const noexcept
        { return _where; }

		bool need_abort(void) const noexcept
        { return _need_abort; }
		
		std::string error_message(void) const noexcept
        { return "'" + _what + "' in '" + _where + "'. " + (_need_abort ? "Crashed" : "No crash"); }

};


#endif /* CUSTOM_EXCEPTION_HPP */
