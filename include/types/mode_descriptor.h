#ifndef __DAB_TYPES__MODE_DESCRIPTOR
#define __DAB_TYPES__MODE_DESCRIPTOR

#include "types/transmission_mode.h"

#include <type_traits>

/**
 * @internal
 * @file
 * @author Felix Morgner
 *
 * @brief This file contains the definition of the template base for DAB transmission modes
 */

namespace dab
  {

  namespace __internal_common
    {

    namespace types
      {

      namespace detail
        {
        constexpr std::size_t next_power_of_two(std::size_t const value, std::size_t const start = 1)
          {
          return start > value ? start : next_power_of_two(value, start * 2);
          }
        }

      /**
       * @internal
       * @brief This class is the a template base for the different DAB modes
       *
       * @tparam Carriers The number of OFDM subcarriers for the transfer mode
       * @tparam FrameSymbols The number of OFDM symbols, including the PR, for each frame
       * @tparam FicSymbols The number of OFDM symbols that make up the FIC
       * @tparam FrameFibs The number of FIBs per frame for the transfer mode
       * @tparam FrameCifs The number of CIFs that make up the MSC of the frame
       *
       * This class provides a number of type members that contain compile-time constant
       * information related to the different modes supported by DAB. There exist some
       * specializations in the file modes.h that represent the 4 currently standardized
       * DAB transfer modes.
       *
       * @note This class is not thought to be used for instanciating object. Even though it
       * would be possible, functions would require to be templated. We therefore encourage the
       * use of values of type dab::transmission_mode_t to specify modes. There exists a
       * couple of * free functions to retrieve mode related information via
       * dab::transmission_mode_t values. We chose this design as it allows clean
       * compiletime computation without a large amount of cryptic nested if/elses.
       */
      template<std::size_t Carriers,
               std::size_t FrameSymbols,
               std::size_t FicSymbols,
               std::size_t FrameFibs,
               std::size_t FrameCifs,
               std::size_t FrameDuration,
               std::size_t GuardDuration,
               std::size_t SymbolDuration,
               std::size_t NullDuration>
      struct mode_descriptor
        {
        /**
         * @internal
         *
         * @brief The number of carriers for the selected transport mode.
         *
         * Each transport mode is associated with a different number of OFDM subcarriers. The
         * @p value member of this type provides access to the number of OFDM subcarriers.
         */
        using carriers = std::integral_constant<std::size_t, Carriers>;

        /**
         * @internal
         *
         * @brief The number of symbols that make up a single frame.
         *
         * Each DAB transport frame is made up of a certain number of OFDM symbols. This type alias
         * provides a member called @p value that allows access to the number of symbols per frame.
         *
         * @note This typedef reduces the actual supplied number by 1, since we strip away the
         * phase-reference symbol.
         */
        using frame_symbols = std::integral_constant<std::size_t, FrameSymbols - 1>;

        /**
         * @internal
         *
         * @brief The number of symbols that make up the Fast Information Channel (FIC).
         *
         * The Fast Information Channel (FIC) is made up of a certain number of symbols, depending
         * on the transfer mode. The FIC carries information describing the DAB ensemble that is
         * being broadcast on a specific frequency.
         */
        using fic_symbols = std::integral_constant<std::size_t, FicSymbols>;

        /**
         * @internal
         *
         * @brief The number of symbols that make up the Main Service Channel (MSC).
         *
         * The number of symbols making up the Main Service Channel (MSC) is dependent upon the
         * transfer mode. The number of MSC symbols is calculated directly from the value carried in
         * frame_symbols and fic_symbols.
         */
        using msc_symbols = std::integral_constant<std::size_t, frame_symbols::value - FicSymbols>;

        /**
         * @internal
         *
         * @brief The number of Fast Information Blocks (FIBs) in a single frame.
         *
         * Each DAB transfer mode use a different number of Fast Information Blocks. These blocks
         * carry the so called Fast Information Groups, which in turn carry the actual information
         * describing the structure of the DAB ensemble.
         */
        using frame_fibs = std::integral_constant<std::size_t, FrameFibs>;

        /**
         * @internal
         *
         * @brief The number of Common Interleaved Frames (CIFs) contained in a single frame.
         *
         * A DAB frame, or rather the MSC of a DAB frame, is made up of a mode dependent number of
         * Common Interleaved Frames (CIFs). These CIFs contain the so-called Capacity Units (CUs)
         * which in turn carry the services of an ensemble.
         */
        using frame_cifs = std::integral_constant<std::size_t, FrameCifs>;

        /**
         * @internal
         *
         * @brief The number of bits that make up a FIB codeword.
         *
         * The FIBs are made up of convolutionally coded codewords. Each group of codewords makes
         * up a FIB and describes a single CIF. The number of FIB bits is equal to the number of
         * FIBs times 256 bit per byte devided by the number of cifs.
         */
        using fib_codeword_bits = std::integral_constant<std::size_t, frame_fibs::value * 256 / frame_cifs::value>;

        /**
         * @internal
         *
         * @brief The number of bits in a symbol.
         *
         * Each OFDM symbol in DAB transports 2 bits of information due to the QPSK used in DAB.
         */
        using symbol_bits = std::integral_constant<std::size_t, carriers::value * 2>;

        /**
         * @internal
         *
         * @brief The length of the FFT used to demodulate the signal
         */
        using fft_length = std::integral_constant<std::size_t, detail::next_power_of_two(carriers::value)>;

        /**
         * @internal
         *
         * @brief The duration of a frame in microseconds
         */
        using frame_duration = std::integral_constant<std::size_t, FrameDuration>;

        /**
         * @internal
         *
         * @brief The duration of the guard in microseconds
         */
        using guard_duration = std::integral_constant<std::size_t, GuardDuration>;

        /**
         * @internal
         *
         * @brief The duration of a symbol without guard in microseconds
         */
        using symbol_duration = std::integral_constant<std::size_t, SymbolDuration>;

        /**
         * @internal
         *
         * @brief The duration of the null symbol in microseconds
         */
        using null_duration = std::integral_constant<std::size_t, NullDuration>;
        };

      /**
       * @internal
       * @ingroup dab_modes
       * @brief DAB transport mode 1 descriptor
       *
       * This type alias generates the descriptor for DAB transport mode 1.
       */
      using mode_1 = mode_descriptor<1536,  76, 3, 12, 4, 96000, 246, 1000, 1297>;

      /**
       * @internal
       * @ingroup dab_modes
       * @brief DAB transport mode 2 descriptor
       *
       * This type alias generates the descriptor for DAB transport mode 2.
       */
      using mode_2 = mode_descriptor< 384,  76, 3,  3, 1, 24000,  62,  250, 324>;

      /**
       * @internal
       * @ingroup dab_modes
       * @brief DAB transport mode 3 descriptor
       *
       * This type alias generates the descriptor for DAB transport mode 3.
       */
      using mode_3 = mode_descriptor< 192, 153, 8,  4, 1, 24000, 31,  125, 168>;

      /**
       * @internal
       * @ingroup dab_modes
       * @brief DAB transport mode 4 descriptor
       *
       * This type alias generates the descriptor for DAB transport mode 4.
       */
      using mode_4 = mode_descriptor< 768,  76, 3,  6, 2, 48000, 123, 500, 648>;

      }

    }

  }

#endif

