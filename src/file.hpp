#ifndef _FILE_HPP_
#define _FILE_HPP_

#include <string>
#include <vector>

class File
{
public:
    File(const std::wstring& filename);
    
    std::vector<std::wstring> read();

    std::wstring getFilename() const;

    bool exist() const;

    void setFileName(const std::wstring& filename);

    void write(const std::wstring& content);

private:
    std::wstring m_filename;
};


#endif // _FILE_HPP_
