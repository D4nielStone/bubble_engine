#include "nucleo/projeto.hpp"
#include "nucleo/fase.hpp"
#include "os/janela.hpp"
#include "filesystem"
#include "string"
#include "iostream"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h> 
bubble::projeto::projeto(const std::string &diretorio) : diretorioDoProjeto(diretorio)
{
    projeto_atual = this;
    std::string full_path = diretorio + "/config";
    if(!std::filesystem::exists(full_path)) 
    {
        depuracao::emitir(erro, "Arquivo de projeto não encontrado!");
        return;
    }
    std::ifstream file(full_path);
    std::stringstream sb;
    
    sb << file.rdbuf();

    rapidjson::Document doc;
    doc.Parse(sb.str().c_str());

    /*      ERROS     */
    if(doc.HasParseError()) 
    {
        depuracao::emitir(erro, "Parse do projeto!");
        return;
    }

    if(!doc.HasMember("lancamento") || !doc["lancamento"].IsString())
    {
        depuracao::emitir(erro, "Defina fase de lancamento em config!");
        return;
    }
    if(!doc.HasMember("janela") || !doc["janela"].IsObject())
    {
        depuracao::emitir(erro, "Defina janela em config!");
        return;
    }
    if(!doc["janela"].GetObject().HasMember("largura") || !doc["janela"].GetObject()["largura"].IsInt())
    {
        depuracao::emitir(erro, "Defina largura da janela em config!");
        return;
    }   
    if(!doc["janela"].GetObject().HasMember("altura") || !doc["janela"].GetObject()["altura"].IsInt())
    {
        depuracao::emitir(erro, "Defina altura da janela em config!");
        return;
    }   
    if(!doc["janela"].GetObject().HasMember("titulo") || !doc["janela"].GetObject()["titulo"].IsString())
    {
        depuracao::emitir(erro, "Defina titulo da janela em config!");
        return;
    }    
    if(!doc["janela"].GetObject().HasMember("icone") || !doc["janela"].GetObject()["icone"].IsString())
    {
        depuracao::emitir(erro, "Defina icone da janela em config!");
        return;
    }   
    /*              */


    const char* nome_janela = doc["janela"].GetObject()["titulo"].GetString();
    std::string icon_path = doc["janela"].GetObject()["icone"].GetString();

    instanciaJanela = new bubble::janela(nome_janela,
     bubble::vetor2<double>(doc["janela"].GetObject()["largura"].GetInt(), doc["janela"].GetObject()["altura"].GetInt()),
    (diretorio + "/" + icon_path).c_str());

	projeto_atual->fase_atual = std::make_shared<bubble::fase>(diretorio + "/" + doc["lancamento"].GetString() + ".fase");
	projeto_atual->fase_atual->carregar();
}

void bubble::projeto::rodar()
{
    while(!glfwWindowShouldClose(instanciaJanela->window))
	{
		instanciaJanela->poll();

		projeto_atual->fase_atual->atualizar(0.01666);

		instanciaJanela->swap();
	}
}
void bubble::projeto::fase(const std::string &nome)
{
    bubble::file_de_tarefas.push([this, nome]()
    {
    	projeto_atual->fase_atual->descarregar();
    	projeto_atual->fase_atual = std::make_shared<bubble::fase>(diretorioDoProjeto + "/" + nome + ".fase");
	projeto_atual->fase_atual->iniciar();
    }
    );
}
std::shared_ptr<bubble::fase> bubble::projeto::obterFaseAtual()
{
	return fase_atual;
}
