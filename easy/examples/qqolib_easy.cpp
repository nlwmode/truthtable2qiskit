#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <assert.h>

#include "kitty/kitty.hpp"
#include "lorina/lorina.hpp"

#include "easy/esop/esop.hpp"
#include "easy/esop/esop_from_pkrm.hpp"
#include "easy/esop/esop_from_pprm.hpp"
#include "easy/esop/exact_synthesis.hpp"

#include "easy/io/write_esop.hpp"

int main( int argc, char** argv )
{
  /** Usages
   *  ./qqolib <argc_1> <argc_2>
   *    <argc_1> refers to the variables numver of a Boolean function
   *    <argc_2> refers to a binary boolean function
   */
  printf( "QQOLIB easy!\n" );
  assert( argc == 3 );

  uint32_t N = std::atoi( argv[1] );
  using TT = kitty::dynamic_truth_table;

  const std::string stt = argv[2];
  TT btt( N );
  kitty::create_from_binary_string<TT>( btt, stt );

  easy::esop::esop_t esop1 = easy::esop::esop_from_optimum_pkrm( btt );
  easy::esop::esop_t esop2 = easy::esop::esop_from_pprm( btt );
  easy::esop::esops_t esop3s = easy::esop::exact_esop( btt );

  easy::write_esop( stt + "_pkrm.pla", esop1, N );
  easy::write_esop( stt + "_pprm.pla", esop2, N );

  for ( uint i = 0; i < esop3s.size(); ++i )
  {
    easy::write_esop( stt + "_exact" + std::to_string( i ) + ".pla", esop3s[i], N );
  }

  return 0;
}