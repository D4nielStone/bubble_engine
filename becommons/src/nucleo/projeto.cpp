#include "nucleo/sistema_de_interface.hpp"
#include "nucleo/projeto.hpp"
#include "nucleo/fase.hpp"
#include "os/janela.hpp"
#include "filesystem"
#include "string"
#include <iostream>
#include <rapidjson/rapidjson.h>
#include <memory>
#include <rapidjson/document.h> 
#include <fstream>

#include "nucleo/sistema_de_fisica.hpp"
#include "nucleo/sistema_de_renderizacao.hpp"
#include "nucleo/sistema_de_codigo.hpp"

using namespace bubble;

void projeto::rodar()
{
    while(!glfwWindowShouldClose(instanciaJanela->window))
	{
		instanciaJanela->poll();
        if(fase_atual->rodando){
        sistemas["fisica"]->atualizar();        
        sistemas["codigo"]->atualizar();        
        }
        sistemas["render"]->atualizar();        
        sistemas["interface"]->atualizar();        

		instanciaJanela->swap();
	}
}

projeto::projeto(const std::string &diretorio, const modoDeExecucao m) : diretorioDoProjeto(diretorio)
{
    auto doc = obterDoc();
    criarJanela(doc);
}

rapidjson::Document projeto::obterDoc()
{
    // torna projeto atual
    if(projeto_atual) delete projeto_atual;
    projeto_atual = this;
    std::string full_path = diretorioDoProjeto + "/config";
    if(!std::filesystem::exists(full_path)) 
    {
        depuracao::emitir(erro, "Arquivo de projeto não encontrado!");
        abort();
    }

    // executa o parsing
    std::ifstream file(full_path);
    std::stringstream sb;
    
    sb << file.rdbuf();

    rapidjson::Document doc;
    doc.Parse(sb.str().c_str());

    return doc;
}

void projeto::criarJanela(rapidjson::Document& doc)
{
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
    (diretorioDoProjeto + "/" + icon_path).c_str());

    fase(diretorioDoProjeto + doc["lancamento"].GetString());
}

void projeto::fase(const std::string &nome)
{
    depuracao::emitir(info, "carregando fase em: " + nome + ".fase");
    
    fase_atual = std::make_shared<bubble::fase>(nome + ".fase");
    fase_atual->carregar();
    iniciarSistemasRuntime(fase_atual.get());
}

void projeto::iniciarSistemasRuntime(bubble::fase* f)
{
    sistemas["fisica"] = new sistema_fisica();
    sistemas["interface"] = new sistema_interface();
    sistemas["render"] = new sistema_renderizacao();
    sistemas["codigo"] = new sistema_codigo();

    for(auto& sistema : sistemas)
    {
        sistema.second->inicializar(f);
    }
}
void projeto::iniciarSistemasEditor(bubble::fase* f)
{
    sistemas["fisica"] = new sistema_fisica();
    sistemas["interface"] = new sistema_interface();
    sistemas["render"] = new sistema_renderizacao();

    for(auto& sistema : sistemas)
    {
        sistema.second->inicializar(f);
    }
}

std::shared_ptr<fase> projeto::obterFaseAtual()
{
	return fase_atual;
}

sistema_fisica* projeto::sfisica()
{
    return dynamic_cast<sistema_fisica*>(sistemas["fisica"]);
}
