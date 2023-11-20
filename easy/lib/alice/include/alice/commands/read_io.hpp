/* alice: C++ command shell library
 * Copyright (C) 2017-2018  EPFL
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*
  \file read_io.hpp
  \brief Generic command for reading I/O

  \author Mathias Soeken
*/

#pragma once

#include <string>

#include <fmt/format.h>

#include "../command.hpp"
#include "../detail/utils.hpp"
#include "../validators.hpp"

namespace alice
{

template<typename Tag, typename S>
int add_read_io_option_helper( command& cmd, unsigned& option_count, std::string& default_option )
{
  if ( can_read<S, Tag>( cmd ) )
  {
    constexpr auto option = store_info<S>::option;

    option_count++;
    default_option = option;
    add_option_helper<S>( cmd.opts );
  }

  return 0;
}

template<typename Tag, typename S>
int read_io_helper( const command& cmd, const std::string& default_option, const environment::ptr& env, const std::string& filename )
{
  constexpr auto option = store_info<S>::option;

  if ( cmd.is_set( option ) || option == default_option )
  {
    if ( cmd.is_set( "new" ) || env->store<S>().empty() )
    {
      env->store<S>().extend();
    }

    env->store<S>().current() = read<S, Tag>( filename, cmd );
  }
  return 0;
}

template<class Tag, class... S>
class read_io_command : public command
{
public:
  read_io_command( const environment::ptr& env, const std::string& name )
      : command( env, fmt::format( "Read {} file", name ) )
  {
    []( ... ) {}( add_read_io_option_helper<Tag, S>( *this, option_count, default_option )... );
    if ( option_count != 1u )
    {
      default_option.clear();
    }

    add_option( "filename,--filename", filename, "filename" )->check( ExistingFileWordExp )->required();
    add_flag( "-n,--new", "create new store entry" );
  }

protected:
  rules validity_rules() const
  {
    rules rules;

    rules.push_back( {[this]() { return option_count == 1 || exactly_one_true_helper( {is_set( store_info<S>::option )...} ); }, "exactly one store needs to be specified"} );

    return rules;
  }

  void execute()
  {
    []( ... ) {}( read_io_helper<Tag, S>( *this, default_option, env, detail::word_exp_filename( filename ) )... );
  }

private:
  std::string filename;
  unsigned option_count = 0u;
  std::string default_option;
};
}
