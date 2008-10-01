//
// RSADigestEngine.cpp
//
// $Id: //poco/1.3/Crypto/src/RSADigestEngine.cpp#1 $
//
// Library: Crypto
// Package: CryptoCore
// Module:  RSADigestEngine
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


#include "Poco/Crypto/RSADigestEngine.h"
#include <openssl/pem.h>


namespace Poco {
namespace Crypto {


RSADigestEngine::RSADigestEngine(const RSAKey& key):
	_key(key),
	_sig(),
	_sha1()
{
	_sig = DigestEngine::Digest(key.size());
}


RSADigestEngine::~RSADigestEngine()
{
}


unsigned RSADigestEngine::digestLength() const
{
	return _sha1.digestLength();
}


void RSADigestEngine::reset()
{
	_sha1.reset();
	_sig = DigestEngine::Digest(_key.size());
}

	
const DigestEngine::Digest& RSADigestEngine::digest()
{
	return _sha1.digest();
}


const DigestEngine::Digest& RSADigestEngine::signature()
{
	const DigestEngine::Digest& digest = _sha1.digest();
	unsigned int sigLen = _sig.size();
	RSA_sign(NID_sha1, &digest[0], (unsigned int)digest.size(), &_sig[0], &sigLen, _key.impl()->getRSA());
	// truncate _sig to sigLen
	if (sigLen < _sig.size())
		_sig.resize(sigLen);
    return _sig;
}

	
void RSADigestEngine::verify(const DigestEngine::Digest& sig)
{
	const DigestEngine::Digest& digest = _sha1.digest();
	DigestEngine::Digest sigCpy = sig; // copy becausse RSA_verify can modify sigCpy
	int ret = RSA_verify(NID_sha1, &digest[0], (unsigned int)digest.size(), &sigCpy[0], (unsigned int)sigCpy.size(), _key.impl()->getRSA());
	if (ret == 0)
		throw Poco::DataFormatException("Signature does not match");
}


void RSADigestEngine::updateImpl(const void* data, unsigned length)
{
	_sha1.update(data, length);
}


} } // namespace Poco::Crypto
