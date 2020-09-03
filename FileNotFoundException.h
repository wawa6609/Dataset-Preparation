#pragma once
#include <stdexcept>
using namespace std;
class FileNotFoundException : virtual public runtime_error {

protected:

    int error_number;               ///< Error number
    int error_offset;               ///< Error offset

public:

    /** Constructor (C++ STL string, int, int).
     *  @param msg The error message
     *  @param err_num Error number
     *  @param err_off Error offset
     */
    explicit
        FileNotFoundException(const string& msg, int err_num, int err_off);

    /** Destructor.
     *  Virtual to allow for subclassing.
     */
    virtual ~FileNotFoundException() throw () {}

    /** Returns error number.
     *  @return #error_number
     */
    virtual int getErrorNumber() const throw() {
        return error_number;
    }

    /**Returns error offset.
     * @return #error_offset
     */
    virtual int getErrorOffset() const throw() {
        return error_offset;
    }

};