#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

#define VS_OUT "struct VSOut\n{\nfloat3 colour : Colour;\nfloat2 uv : UV;\nfloat4 pos : SV_Position;\n};\n VSOut"




class WizardShaderLanguage
{
    
public:
    static std::string compileToHLSL(std::string code)
    {
        auto tokens=tokenise(code);
        std::string compiled;
        for (int i = 0; i < tokens.size();i++)
        {
            if (tokens[i] == "func")
            {
                int x = find(tokens.begin(),tokens.end(),"}")-tokens.begin();
                compiled+=funcComplie(tokens.begin()+i,tokens.begin()+x+1);
            }
        }
        return compiled;
    }

private:
    static std::string funcComplie(std::vector<std::string>::iterator start, std::vector<std::string>::iterator end)
    {
        static constexpr char returnT='@';
        std::string out;
        std::cout<<*(start+1)<<'\n';
        if (
            *(start+1)=="vertex"
        )
        {
            out += VS_OUT;
            out += " main";
        }
        else
        {
            out += returnT;
            out += std::move(*(start+1));
        }
        

        for(start+=2;start<end;start++)
        {
            out+=' ';
            out += std::move(*start);
        }
        return out;
    }

    static std::vector<std::string> tokenise(std::string code)
    {
        std::vector<std::string> tokens;
        std::string token;
        tokens.reserve(20);
        token.reserve(5);
        for (auto i : code)
        {
            switch (i)
            {
                case ' ':
                    if (token!="") tokens.push_back(token);
                    token.clear();
                    continue;
                    break;
                case '\t':
                token+="    ";
                    continue;
                    break;
                case '\n':
                    if (token!="") tokens.push_back(token);
                    tokens.push_back("\n");
                    token.clear();
                    continue;
                    break;
                case '}':
                    if (token!="") tokens.push_back(token);
                    tokens.push_back("}");
                    token.clear();
                    continue;
                    break;
            }
            token+=i;
            //end:

        }
        return tokens;
    }

    static std::string getSemantics(std::string name)
    {
        for (auto& i : name)
        {
            if ('a'<=i && i<='z') i+= 'A'-'a';
        }
        name += "_WSL";
        return name;
    }
};




#if 1

int main()
{
    std::string code;

    std::cout << "Enter your code\n";
    std::ifstream in("test.txt");

			while (!in.eof())
			{
				std::string tp;
				std::getline(in, tp);
				code += tp;
				code += '\n';
			}
    
    std::cout<<"Your code is compiled to this :\n"<<WizardShaderLanguage::compileToHLSL(code);

    return 0;
}

#endif