#pragma once


namespace skl{

	template<class _Ty>
	class scope_guard
	{
	public:
		scope_guard(_Ty onExitScope)
			: _on_exit(onExitScope), _dismissed(false)
		{ }

		~scope_guard()
		{
			if (!_dismissed)
			{
				_on_exit();
			}
		}

		void Dismiss()
		{
			_dismissed = true;
		}

	private:
		_Ty _on_exit;
		bool _dismissed;
	private: // noncopyable
		scope_guard(scope_guard const&);
		scope_guard& operator=(scope_guard const&);
		scope_guard(scope_guard&&);
		scope_guard& operator=(scope_guard&&);
	};
#ifndef SCOPE_GUARD_LAMBDA
#define SCOPE_GUARD_LAMBDA(Name, Lambda) auto Unique##Name = Lambda; skl::scope_guard<decltype(Unique##Name)> Name(Unique##Name)
#endif
}
