//
// Cipher.h
//
// $Id: //poco/1.3/Crypto/include/Poco/Crypto/Cipher.h#1 $
//
// Library: Crypto
// Package: CryptoCore
// Module:  Cipher
//
// Definition of the Cipher class.
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


#ifndef Crypto_Cipher_INCLUDED
#define Crypto_Cipher_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"

#include <vector>


namespace Poco {
namespace Crypto {


class CryptoTransform;


class Crypto_API Cipher: public Poco::RefCountedObject
	/// Represents the abstract base class from which all implementations of
	/// symmetric/assymetric encryption algorithms must inherit.  Use the CipherFactory
	/// class to obtain an instance of this class:
	///
	///     CipherFactory& factory = CipherFactory::defaultFactory();
	///     // Creates a 256-bit AES cipher
	///     Cipher* pCipher = factory.createCipher(CipherKey("aes-256"));
	///     Cipher* pRSACipher = factory.createCipher(RSAKey(RSAKey::KL_1024, RSAKey::EXP_SMALL));
	///
	/// Check the different Key constructors on how to initialize/create
	/// a key. The above example auto-generates random keys.
	///
	/// Note that you won't be able to decrypt data encrypted with a random key
	/// once the Cipher is destroyed unless you persist the generated key and IV.
	/// An example usage for random keys is to encrypt data saved in a temporary
	/// file.
	///
	/// Once your key is set up, you can use the Cipher object to encrypt or
	/// decrypt strings or, in conjunction with a CryptoInputStream or a
	/// CryptoOutputStream, to encrypt streams of data.
	///
	/// Since encrypted strings will contain arbitary binary data that will cause
	/// problems in applications that are not binary-safe (eg., when sending
	/// encrypted data in e-mails), the encryptString() and decryptString() can
	/// encode (or decode, respectively) encrypted data using a "transport encoding".
	/// Supported encodings are Base64 and BinHex.
	///
	/// The following example encrypts and decrypts a string utilizing Base64
	/// encoding:
	///
	///     std::string plainText = "This is my secret information";
	///     std::string encrypted = pCipher->encryptString(plainText, Cipher::ENC_BASE64);
	///     std::string decrypted = pCipher->decryptString(encrypted, Cipher::ENC_BASE64);
	///
	/// In order to encrypt a stream of data (eg. to encrypt files), you can use
	/// a CryptoStream:
	///
	///     // Create an output stream that will encrypt all data going through it
	///     // and write pass it to the underlying file stream.
	///     Poco::FileOutputStream sink("encrypted.dat");
	///     CryptoOutputStream encryptor(sink, pCipher->createEncryptor());
	///     
	///     Poco::FileInputStream source("source.txt");
	///     Poco::StreamCopier::copyStream(source, encryptor);
	///     
	///     // Always close output streams to flush all internal buffers
	///     encryptor.close();
	///     sink.close();
{
public:
	typedef Poco::AutoPtr<Cipher> Ptr;
	typedef std::vector<unsigned char> ByteVec;

	enum Encoding
		/// Transport encoding to use for encryptString() and decryptString().
	{
		ENC_NONE,		/// Plain binary output
		ENC_BASE64,		/// Base64-encoded output
		ENC_BINHEX		/// BinHex-encoded output
	};

	virtual ~Cipher();
		/// Destroys the Cipher.

	virtual const std::string& name() const = 0;
		/// Returns the name of the Cipher.

	virtual CryptoTransform* createEncryptor() = 0;
		/// Creates an encrytor object to be used with a CryptoStream.

	virtual CryptoTransform* createDecryptor() = 0;
		/// Creates a decryptor object to be used with a CryptoStream.

	virtual std::string encryptString(const std::string& str, Encoding encoding = ENC_NONE);
		/// Directly encrypt a string and encode it using the given encoding.

	virtual std::string decryptString(const std::string& str, Encoding encoding = ENC_NONE);
		/// Directly decrypt a string that is encoded with the given encoding.

	virtual void encrypt(std::istream& source, std::ostream& sink, Encoding encoding = ENC_NONE);
		/// Directly encrypts an input stream and encodes it using the given encoding.

	virtual void decrypt(std::istream& source, std::ostream& sink, Encoding encoding = ENC_NONE);
		/// Directly decrypt an input stream that is encoded with the given encoding.

protected:
	Cipher();
		/// Creates a new Cipher object.

private:
	Cipher(const Cipher&);
	Cipher& operator = (const Cipher&);
};


} } // namespace Poco::Crypto


#endif // Crypto_Cipher_INCLUDED
