//
// MediaType.cpp
//
// $Id: //poco/Main/Net/src/MediaType.cpp#8 $
//
// Library: Net
// Package: Messages
// Module:  MediaType
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
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


#include "Poco/Net/MediaType.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/String.h"
#include <algorithm>
#include <cctype>


using Poco::icompare;


namespace Poco {
namespace Net {


MediaType::MediaType(const std::string& mediaType)
{
	parse(mediaType);
}

	
MediaType::MediaType(const std::string& type, const std::string& subType):
	_type(type),
	_subType(subType)
{
}


MediaType::MediaType(const MediaType& mediaType):
	_type(mediaType._type),
	_subType(mediaType._subType),
	_parameters(mediaType._parameters)
{
}


MediaType::~MediaType()
{
}


MediaType& MediaType::operator = (const MediaType& mediaType)
{
	if (&mediaType != this)
	{
		_type       = mediaType._type;
		_subType    = mediaType._subType;
		_parameters = mediaType._parameters;
	}
	return *this;
}

	
MediaType& MediaType::operator = (const std::string& mediaType)
{
	parse(mediaType);
	return *this;
}


void MediaType::swap(MediaType& mediaType)
{
	std::swap(_type, mediaType._type);
	std::swap(_subType, mediaType._subType);
	_parameters.swap(mediaType._parameters);
}

	
void MediaType::setType(const std::string& type)
{
	_type = type;
}

	
void MediaType::setSubType(const std::string& subType)
{
	_subType = subType;
}

	
void MediaType::setParameter(const std::string& name, const std::string& value)
{
	_parameters.set(name, value);
}

	
const std::string& MediaType::getParameter(const std::string& name) const
{
	return _parameters.get(name);
}

	
bool MediaType::hasParameter(const std::string& name) const
{
	return _parameters.has(name);
}

	
void MediaType::removeParameter(const std::string& name)
{
	_parameters.erase(name);
}

	
std::string MediaType::toString() const
{
	std::string result;
	result.append(_type);
	result.append("/");
	result.append(_subType);
	for (NameValueCollection::ConstIterator it = _parameters.begin(); it != _parameters.end(); ++it)
	{
		result.append("; ");
		result.append(it->first);
		result.append("=");
		MessageHeader::quote(it->second, result);
	}
	return result;
}


bool MediaType::matches(const MediaType& mediaType) const
{
	return matches(mediaType._type, mediaType._subType);
}

	
bool MediaType::matches(const std::string& type, const std::string& subType) const
{
	return icompare(_type, type) == 0 && icompare(_subType, subType) == 0;
}


bool MediaType::matches(const std::string& type) const
{
	return icompare(_type, type) == 0;
}


void MediaType::parse(const std::string& mediaType)
{
	_type.clear();
	_subType.clear();
	_parameters.clear();
	std::string::const_iterator it  = mediaType.begin();
	std::string::const_iterator end = mediaType.end();
	while (it != end && std::isspace(*it)) ++it;
	while (it != end && *it != '/') _type += *it++;
	if (it != end) ++it;
	while (it != end && *it != ';' && !std::isspace(*it)) _subType += *it++;
	while (it != end && *it != ';') ++it;
	MessageHeader::splitParameters(it, end, _parameters);
}


} } // namespace Poco::Net
