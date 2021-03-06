// Copyright (C) 2010 by Thomas Moulard, CNRS.
//
// This file is part of the jrl-dynamics.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#include <jrl/dynamics/config.hh>

#ifndef JRL_DYNAMICS_ASSERTION_HH
# define JRL_DYNAMICS_ASSERTION_HH
# include <boost/scope_exit.hpp>

/// \brief Launch an exception.
/* # define JRL_DYNAMICS_THROW_EXCEPTION_(MSG)			\
throw ::dynamicsJRLJapan::Exception (MSG, __FILE__, __LINE__)
*/

/// \brief Launch an exception (generic macro).
# define JRL_DYNAMICS_THROW_EXCEPTION(TYPE, MSG) \
throw TYPE (MSG, __FILE__, __LINE__)

/// \brief Define a custom exception.
# define JRL_DYNAMICS_MAKE_EXCEPTION(EXTRA_QUALIFIER, TYPE) \
class EXTRA_QUALIFIER TYPE : public std::logic_error \
{ \
public: \
TYPE (const std::string& message, \
const std::string& file, \
unsigned line) throw () \
	: std::logic_error (file + ": " + message) \
{} \
}

// If debug mode is disabled and assertions are not already
// disabled, disable them automatically.
//# if (defined JRL_DYNAMICS_DEBUG) && (!defined JRL_DYNAMICS_ENABLE_ASSERTIONS)
#  define JRL_DYNAMICS_ENABLE_ASSERTIONS
//# endif // (!defined JRL_DYNAMICS_DEBUG) && (!defined JRL_DYNAMICS_ENABLE_ASSERTIONS)

namespace dynamicsJRLJapan
{
  JRL_DYNAMICS_MAKE_EXCEPTION (JRL_DYNAMICS_DLLLOCAL, AssertionError);
} // end of namespace dynamicsJRLJapan.

/// \brief Define JRL_DYNAMICS_ASSERT.
///
/// Throw an ::dynamicsJRLJapan::AssertionError if macro argument evaluates to
/// false.
# ifdef JRL_DYNAMICS_ENABLE_ASSERTIONS
#  define JRL_DYNAMICS_ASSERT(CONDITION)					\
  do {								\
    bool _x = (CONDITION);					\
    if (!_x)							\
      JRL_DYNAMICS_THROW_EXCEPTION					\
	(::dynamicsJRLJapan::AssertionError,					\
	 #CONDITION " evaluates to false");			\
  } while (0)
# else
#  define JRL_DYNAMICS_ASSERT(CONDITION)
# endif // JRL_DYNAMICS_ENABLE_ASSERTIONS

/// \brief Define macro for precondition checking.
# define JRL_DYNAMICS_PRECONDITION(CONDITION) JRL_DYNAMICS_ASSERT (CONDITION)

/// \brief Define macro for postcondition checking.
///
/// This macro allows an assertion to be done whenever a
/// scope is exited.
///
/// This macro takes two arguments: the condition which should
/// be evaluated and a list of captured variables.
/// All variables used in the condition must be in this list.
///
/// They can either be copied or taken by reference:
/// JRL_DYNAMICS_POSTCONDITION (a == 3, (a))
/// ...checks that a is equal to three and copy the variable a
/// when JRL_DYNAMICS_POSTCONDITION is called.
///
/// JRL_DYNAMICS_POSTCONDITION (a == 3, (&a))
/// ...takes a by reference and evaluated its final value (when
/// the scope is exited).
///
/// See Boost.ScopeExit for more details.
# define JRL_DYNAMICS_POSTCONDITION(CONDITION, CAPTURED_VARIABLES)	\
  BOOST_SCOPE_EXIT(CAPTURED_VARIABLES)				\
  {								\
    JRL_DYNAMICS_ASSERT (CONDITION);					\
  } BOOST_SCOPE_EXIT_END					\
  struct _e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n

# define GENERATE_TEST()									\
int main()													\
{															\
	try														\
	{														\
		run_test();											\
	}														\
	catch (const AssertionError& exception)					\
	{														\
		std::cout << "The test has failed: " << std::endl	\
				  << exception.what() << std::endl;			\
		return 1;											\
	}														\
	catch (const char* msg)									\
	{														\
		std::cout << "The test has failed: " << std::endl	\
				  << msg << std::endl;						\
		return 2;											\
	}														\
}

#endif //! JRL_DYNAMICS_ASSERTION_HH
