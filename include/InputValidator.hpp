#ifndef INPUT_VALIDATOR_HPP
#define INPUT_VALIDATOR_HPP

#include <iosfwd>
#include <string>

namespace InputValidator {

// Pure parser: true only if the whole string is an integer within [minVal, maxVal].
bool parseInt(const std::string& text, int minVal, int maxVal, int& result);

// Prompts until a valid integer is entered.
// Returns false only if input ends (EOF / closed stdin) so callers can exit cleanly.
bool readInt(std::istream& in, std::ostream& out, const std::string& prompt,
             int minVal, int maxVal, int& result);

} // namespace InputValidator

#endif // INPUT_VALIDATOR_HPP
