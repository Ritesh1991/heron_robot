/**
 *      _____
 *     /  _  \
 *    / _/ \  \
 *   / / \_/   \
 *  /  \_/  _   \  ___  _    ___   ___   ____   ____   ___   _____  _   _
 *  \  / \_/ \  / /  _\| |  | __| / _ \ | ++ \ | ++ \ / _ \ |_   _|| | | |
 *   \ \_/ \_/ /  | |  | |  | ++ | |_| || ++ / | ++_/| |_| |  | |  | +-+ |
 *    \  \_/  /   | |_ | |_ | ++ |  _  || |\ \ | |   |  _  |  | |  | +-+ |
 *     \_____/    \___/|___||___||_| |_||_| \_\|_|   |_| |_|  |_|  |_| |_|
 *             ROBOTICS™ 
 *
 *  File: port.h
 *  Desc: Provides the Port class, which acts as one of the two endpoints
 *        of the vserial utility.  Currently, a port may be either a 
 *        pseudo terminal or a real serial port.
 *  Auth: Iain Peet
 *
 *  Copyright (c) 2010, Clearpath Robotics, Inc. 
 *  All Rights Reserved
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Clearpath Robotics, Inc. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CLEARPATH ROBOTICS, INC. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Please send comments, questions, or patches to skynet@clearpathrobotics.com 
 *
 */

#ifndef VSERIAL_PORT_H
#define VSERIAL_PORT_H

class Validator;

/** Port abstract base class.  
 *  Provides the static create() function, which will instantiate the proper
 *  class type for a particular connection string */
class Port {
protected:
    const char* m_conn_string;
    Validator*  m_validator;

    Port(const char *conn_string) : m_conn_string(conn_string), m_validator(0) 
        {}

    virtual int  _read(unsigned char * ch) = 0;
    virtual void _write(unsigned char ch) =  0;

public:
    virtual ~Port();

    static Port* create(const char *conn_string);
   
    int readByte(unsigned char *ch);
    void writeByte(unsigned char ch);

    void setValidator(Validator * v) { m_validator = v; }
};

/** Pseudo terminal port class */
class PtyPort: public Port {
private:
    const char* m_path;
    int         m_ptyfd;
    int         m_slavefd;

    void _create_pty();
    void _respawn_pty();

protected:
    virtual int _read(unsigned char * ch);
    virtual void _write(unsigned char ch);

public:
    PtyPort(const char *path);
    virtual ~PtyPort();
};

/** Actual serial port class 
 *  Connection string has the form "serial:devname,baud"  */
class SerialPort: public Port {
protected:
    void* m_serial;

    virtual int _read(unsigned char * ch);
    virtual void _write(unsigned char ch);
public:
    SerialPort(const char *conn_string);
    virtual ~SerialPort();
};

/** For when things go terribly wrong */
class PortException {
public:
    const char* conn_str;
    const char* message;
    PortException(const char* con, const char* msg);
};

#endif // VSERIAL_PORT_H
