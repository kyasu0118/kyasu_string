//  Created by kyasu on 2018/11/23.
//  Copyright 2018 kyasu. All rights reserved.

#ifndef kyasu_string_h
#define kyasu_string_h

#include <string>
#include <vector>
#include <exception>
#include <ctype.h>

namespace kyasu
{
    class string : public std::string
    {
    private:
        class format_id_exception : public std::exception
        {
        private:
            std::string mssage;
        public:
            format_id_exception(const char* message){ this->mssage = std::string(__FUNCTION__) + " " + message; }
            virtual const char* what(){ return mssage.c_str(); }
            virtual const char* what() const noexcept { return mssage.c_str(); }
        };
        
        class buffer_overrun_exception : public std::exception
        {
        private:
            std::string mssage;
        public:
            buffer_overrun_exception(const char* message){ this->mssage = std::string(__FUNCTION__) + " " + message; }
            virtual const char* what(){ return mssage.c_str(); }
            virtual const char* what() const noexcept { return mssage.c_str(); }
        };
    public:
        string(){}
        string( const char* str ) : std::string( str ){}
        string( const std::string& str ) : std::string( str ){}
        operator const char*() const{ return c_str(); }

        std::string substring(int beginIndex, int endIndex) const
        {
            if( beginIndex < 0 || beginIndex > endIndex || endIndex > this->size())
            {
                throw buffer_overrun_exception(
                    string::format(
                        "\"%s\" beginIndex=%d endIndex=%d",
                        c_str(), beginIndex, endIndex).c_str() );
            }
            std::vector< char > result( endIndex - beginIndex + 1 );
            
            for( int i=0; i<result.size()-1; ++i )
            {
                result[i] = (*this)[beginIndex+i];
            }
            return &result[0];
        }

        int indexOf( const std::string& delimiter ) const
        {
            for( int i=0; i<size()-delimiter.size(); ++i )
            {
                bool find = true;
                for( int j=0; j<delimiter.size(); ++j )
                {
                    if( (*this)[i+j] != delimiter[j] )
                    {
                        find = false;
                        break;
                    }
                }
                if( find )
                {
                    return i;
                }
            }
            return -1;
        }

        std::vector<std::string> split( const std::string& delimiter ) const
        {
            std::vector<std::string> result;
            int index = 0;
            string buffer = *this;
            string buffer_temp;
            
            while( (index = buffer.indexOf( delimiter )) >= 0 )
            {
                result.emplace_back( buffer.substring(0, index) );
                buffer_temp = &buffer[ index + delimiter.size() ];
                buffer = buffer_temp;
            }
            result.emplace_back( buffer );
            
            return result;
        }
        
        std::string replace( const std::string& oldString, const std::string& newString ) const
        {
            std::string result;
            int index = 0;
            string buffer = *this;
            string buffer_temp;
            
            while( (index = buffer.indexOf( oldString )) >= 0 )
            {
                result += buffer.substring(0, index) + newString;
                buffer_temp = &buffer[ index + oldString.size() ];
                buffer = buffer_temp;
            }
            return result + buffer;
        }

        std::string toLower() const
        {
            std::vector<char> result( size() + 1 );
            
            for( size_t i=0; i<size(); )
            {
                const size_t count = charCount( &(*this)[i] );
                
                if( count == 1 )
                {
                    result[i]= tolower( (*this)[i] );
                    ++i;
                }
                else
                {
                    for( size_t j=0; j<count; ++i, ++j )
                    {
                        result[i] = (*this)[i];
                    }
                }
            }
            return &result[0];
        }
        
        std::string toUpper() const
        {
            std::vector<char> result( size() + 1 );
            
            for( size_t i=0; i<size(); )
            {
                const size_t count = charCount( &(*this)[i] );
                
                if( count == 1 )
                {
                    result[i]= toupper( (*this)[i] );
                    ++i;
                }
                else
                {
                    for( size_t j=0; j<count; ++i, ++j )
                    {
                        result[i] = (*this)[i];
                    }
                }
            }
            return &result[0];
        }
        
    private:
        static size_t charCount( const char* text )
        {
            size_t result = 1;
            const unsigned char* bin = (const unsigned char*)text;

            if( bin[0] & 0x80 )
            {
                result = 2;
                
                if( (0b11110000 & bin[0]) == 0b11100000 )
                {
                    if( (0b11000000 & bin[1]) == 0b10000000 &&
                        (0b11000000 & bin[2]) == 0b10000000 )
                    {
                        result = 3;
                    }
                }
                else if( (0b11111000 & bin[0]) == 0b11110000 )
                {
                    if( (0b11000000 & bin[1]) == 0b10000000 &&
                        (0b11000000 & bin[2]) == 0b10000000 &&
                        (0b11000000 & bin[3]) == 0b10000000 )
                    {
                        result = 4;
                    }
                }
            }
            return result;
        }
        static std::string formatId( const char* text, size_t& index )
        {
            std::string result;
            while( text[index] != '\0' )
            {
                result += text[index];
                switch( text[index] )
                {
                    case 'c':
                    case 'd':
                    case 'f':
                    case 'o':
                    case 'X':
                    case 'x':
                    case 's':
                        return result;
                }
                ++index;
            }
            throw format_id_exception(result.c_str());
        }
        static size_t formatIdLength( const std::string& id )
        {
            const bool zero_padding = id[1] == '0';
            
            switch( id[id.size()-1] )
            {
                case 'c':
                    return 1;
                case 'd':
                case 'u':
                case 'o':
                case 'x':
                case 'X':
                    if( zero_padding )
                    {
                        return (size_t)atoi( &id[2] ) + 16;
                    }
                    return (size_t)atoi( &id[1] ) + 16;
                case 'f':
                    for( size_t i=1; i<id.size(); ++i )
                    {
                        if( id[i] == '.' )
                        {
                            return (size_t)atoi( &id[i+1] ) + 32;
                        }
                    }
                    return 32;
            }
            throw format_id_exception(id.c_str());
        }
    public:
        static std::string format( const char* format, ... )
        {
            std::string result;
            std::vector<char> buffer(256);

            va_list args;
            va_start(args, format);

            const size_t length = strlen(format);
            for( size_t i=0; i<length; ++i )
            {
                if( format[i] == '%' )
                {
                    const std::string id = formatId(format, i);
                    const char id_back = id[id.size()-1];
                    
                    switch( id_back )
                    {
                        case 'c':
                            result += (char)va_arg(args, int);
                            break;
                        case 'd':
                        case 'u':
                        case 'o':
                        case 'x':
                        case 'X':
                        case 'f':
                            {
                                const size_t bufferSize = formatIdLength(id);
                                if( bufferSize > buffer.size())
                                {
                                    buffer.resize( bufferSize );
                                }
                                if( id_back == 'f' )
                                {
                                    sprintf( &buffer[0], id.c_str(), va_arg(args, double) );
                                }
                                else
                                {
                                    sprintf( &buffer[0], id.c_str(), va_arg(args, int) );
                                }
                                result += &buffer[0];
                            }
                            break;
                        
                        case 's':
                            result += va_arg(args, const char*);
                            break;
                    }
                }
                else
                {
                    const size_t count = charCount( &format[i] );
                    
                    for( size_t j=0; j<count; ++j )
                    {
                        result += format[i+j];
                    }
                    i += count - 1;
                }
            }
            va_end(args);
            
            return result;
        }
    };
}

#endif /* kyasu_string_h */
