//
// CryptoStream.h
//
// $Id: //poco/1.3/Crypto/include/Poco/Crypto/CryptoStream.h#1 $
//
// Library: Crypto
// Package: CryptoCore
// Module:  CryptoStream
//
// Definition of the CryptoStreamBuf, CryptoInputStream and CryptoOutputStream
// classes.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Crypto_CryptoStream_INCLUDED
#define Crypto_CryptoStream_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/BufferedStreamBuf.h"
#include "Poco/Buffer.h"
#include <iostream>


namespace Poco {
namespace Crypto {


class CryptoTransform;
class Cipher;


class Crypto_API CryptoStreamBuf : public Poco::BufferedStreamBuf
	/// This stream buffer performs cryptographic transformation on the data
	/// going through it.
{
public:
	CryptoStreamBuf(std::istream& istr, CryptoTransform* pTransform, std::size_t bufferSize = 8192);
	CryptoStreamBuf(std::ostream& ostr, CryptoTransform* pTransform,
		std::size_t bufferSize = 8192);

	virtual ~CryptoStreamBuf();

	void close();

protected:
	int readFromDevice(char* buffer, std::streamsize length);
	int writeToDevice(const char* buffer, std::streamsize length);

private:
	CryptoTransform* _pTransform;
	std::istream*	 _pIstr;
	std::ostream*	 _pOstr;
	bool			 _eof;

	Poco::Buffer<unsigned char> _buffer;

	CryptoStreamBuf(const CryptoStreamBuf&);
	CryptoStreamBuf& operator = (const CryptoStreamBuf&);
};


class Crypto_API CryptoIOS : public virtual std::ios
	/// The base class for CryptoInputStream and CryptoOutputStream.
	///
	/// This class is needed to ensure correct initialization order of the
	/// stream buffer and base classes.
{
public:
	CryptoIOS(std::istream& istr, CryptoTransform* pTransform,
		std::size_t bufferSize = 8192);
	CryptoIOS(std::ostream& ostr, CryptoTransform* pTransform,
		std::size_t bufferSize = 8192);
	~CryptoIOS();
	CryptoStreamBuf* rdbuf();

protected:
	CryptoStreamBuf _buf;
};


class Crypto_API CryptoInputStream : public CryptoIOS, public std::istream
	/// This stream transforms all data passing through it using the given
	/// CryptoTransform.
	///
	/// Use a CryptoTransform object provided by Cipher::createEncrytor() or
	/// Cipher::createDecryptor() to create an encrypting or decrypting stream,
	/// respectively.
{
public:
	CryptoInputStream(std::istream& istr, CryptoTransform* pTransform, std::size_t bufferSize = 8192);
		/// Create a new CryptoInputStream object. The CryptoInputStream takes the
		/// ownership of the given CryptoTransform object.

	CryptoInputStream(std::istream& istr, Cipher& cipher, std::size_t bufferSize = 8192);
		/// Create a new encrypting CryptoInputStream object using the given cipher.

	~CryptoInputStream();
};


class Crypto_API CryptoOutputStream : public CryptoIOS, public std::ostream
	/// This stream transforms all data passing through it using the given
	/// CryptoTransform.
	///
	/// Use a CryptoTransform object provided by Cipher::createEncrytor() or
	/// Cipher::createDecryptor() to create an encrypting or decrypting stream,
	/// respectively.
	///
	/// After all data has been passed through the stream, close() must be called
	/// to ensure completion of cryptographic transformation.
{
public:
	CryptoOutputStream(std::ostream& ostr, CryptoTransform* pTransform, std::size_t bufferSize = 8192);
		/// Create a new CryptoOutputStream object. The CryptoOutputStream takes the
		/// ownership of the given CryptoTransform object.

	CryptoOutputStream(std::ostream& ostr, Cipher& cipher, std::size_t bufferSize = 8192);
		/// Create a new decrypting CryptoOutputStream object using the given cipher.

	~CryptoOutputStream();

	void close();
};


} } // namespace Poco::Crypto


#endif // Crypto_CryptoStream_INCLUDED
