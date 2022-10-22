#pragma once
#include <string>
class WizardException : public std::exception
{
public:
	WizardException(int line, const char* file) noexcept;
	const char* what() const noexcept override;
	const wchar_t* whatW()const noexcept;
	virtual const char* GetType() const noexcept;
	virtual const wchar_t* GetTypeW() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;
private:
	int line;
	std::string file;
protected:
	mutable std::string whatBuffer;
	mutable wchar_t* str = nullptr;
};

