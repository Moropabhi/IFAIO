#include "WizardException.h"
#include <sstream>

namespace IFAIO
{

    WizardException::WizardException(int line, const char* file) noexcept
        :line(line), file(file)
    { }

    const char* WizardException::what() const noexcept
    {
        std::ostringstream oss;
        oss << GetType() << std::endl
            << GetOriginString();
        whatBuffer = oss.str();
        return whatBuffer.c_str();
    }

    const wchar_t* WizardException::whatW() const noexcept
    {
        what();
        delete[] str;
        str = new wchar_t[whatBuffer.size()];
        for (int i = 0; i < whatBuffer.size(); i++)
            str[i] = wchar_t(whatBuffer[i]);

        return str;
    }

    const char* WizardException::GetType() const noexcept
    {
        return  "WizardException";
    }

    const wchar_t* WizardException::GetTypeW() const noexcept
    {
        return L"WizardException";
    }

    int WizardException::GetLine() const noexcept
    {
        return line;
    }

    const std::string& WizardException::GetFile() const noexcept
    {
        return file;
    }

    std::string WizardException::GetOriginString() const noexcept
    {
        std::ostringstream oss;
        oss << "[File] " << file << std::endl
            << "[Line] " << line;
        return oss.str();
    }

}