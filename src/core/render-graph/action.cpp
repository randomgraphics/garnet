#include "../../inc/garnet/GNrender-graph.h"

namespace GN::rg {

// ============================================================================
// Action::Parameter implementation
// ============================================================================

// Constructor for parameter with type, name, and options
// Options string specifies parameter behavior:
// 'O' or 'o' = optional parameter (not required)
// 'R' or 'r' = reading access (input parameter)
// 'W' or 'w' = writing access (output parameter)
Action::Parameter::Parameter(const Guid & type, const StrA & name, const char * options)
    : type(type)
    , name(name)
{
    if (options) {
        for (const char * opt = options; *opt; opt++) {
            switch (*opt) {
                case 'O':
                case 'o':
                    optional = true;
                    break;
                case 'R':
                case 'r':
                    reading = true;
                    break;
                case 'W':
                case 'w':
                    writing = true;
                    break;
                default:
                    break;
            }
        }
    }
}



} // namespace GN::rg