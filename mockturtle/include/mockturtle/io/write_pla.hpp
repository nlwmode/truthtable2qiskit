#pragma once

#include "kitty/kitty.hpp"
#include <lorina/pla.hpp>
#include <ostream>
#include <sstream>

namespace mockturtle
{

/*! \brief Writes ESOP form in PLA format to output stream
 *
 * \param os Output stream
 * \param esop ESOP form
 * \param num_vars Number of variables
 */
inline void write_esop( std::ostream& os, std::vector<kitty::cube> const& esop, unsigned num_vars )
{
  lorina::pla_writer writer( os );
  writer.on_number_of_inputs( num_vars );
  writer.on_number_of_outputs( 1 );
  writer.on_number_of_terms( esop.size() );
  writer.on_keyword( "type", "esop" );
  for ( const auto& e : esop )
  {
    std::stringstream ss;
    e.print( num_vars, ss );
    writer.on_term( ss.str(), "1" );
  }
  writer.on_end();
}

/*! \brief Writes ESOP form in PLA format to a file
 *
 * \param filename Name of the file to be written
 * \param esop ESOP form
 * \param num_vars Number of variables
 */
inline void write_esop( std::string const& filename, std::vector<kitty::cube> const& esop, unsigned num_vars )
{
  std::ofstream os( filename.c_str(), std::ofstream::out );
  write_esop( os, esop, num_vars );
  os.close();
}

} // namespace mockturtle

// Local Variables:
// c-basic-offset: 2
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'innamespace 0)
// End:
