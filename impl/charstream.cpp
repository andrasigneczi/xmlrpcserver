#include "charstream.h"

CharStream::CharStream( const char* const pcBuffer, ssize_t iDataLength )
{
	Reset( pcBuffer, iDataLength );
}

void CharStream::Reset( const char* const pcBuffer, ssize_t iDataLength )
{
	m_pcBuffer = pcBuffer;
	m_iSize = iDataLength;
}

void CharStream::SetLength( ssize_t iDataLength )
{
	m_iSize = iDataLength;
}

void CharStream::Reset( ssize_t iDataLength )
{
	m_pcBuffer -= iDataLength - m_iSize;
	m_iSize = iDataLength;
}

char CharStream::get() throw( std::out_of_range )
{
	if( m_iSize <= 0 )
		throw std::out_of_range( "CharStream::get" );
	return *m_pcBuffer;
}

ssize_t CharStream::length() const
{
	return m_iSize;
}

void CharStream::operator++() throw( std::out_of_range )
{
	if( m_iSize <= 0 )
		throw std::out_of_range( "CharStream::operator++" );
	m_iSize--;
	m_pcBuffer++;
}

