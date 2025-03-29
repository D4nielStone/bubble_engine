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
#include <rapidjson/writer.h> 
#include <rapidjson/stringbuffer.h> 
#include <unordered_map>
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

        if(fase_atual->rodando)
        sistemas["interface"]->atualizar();        

        for(auto& s : sistemas_adicionais)
        {
            s.second->atualizar();
        }

		instanciaJanela->swap();
	}
}

projeto::~projeto()
{
    for(auto& sistema : sistemas)
    {
        delete sistema.second;
    }
    sistemas.clear();
    sistemas_adicionais.clear();
}

projeto::projeto(const std::string &diretorio) : diretorioDoProjeto(diretorio)
{
    auto doc = obterDoc();
    criarJanela(doc);
}

void projeto::criarProjetoVazio(const std::string& novo_diretorio, const char* nome)
{
        // Cria diretório do projeto
        std::filesystem::create_directories(novo_diretorio + "/" + nome);
        
        diretorioDoProjeto = novo_diretorio + "/" + nome;
        
        // Criar um novo JSON de configuração
        rapidjson::Document newDoc;
        newDoc.SetObject();
        rapidjson::Document::AllocatorType& allocator = newDoc.GetAllocator();

        rapidjson::Value _janela(rapidjson::kObjectType);
        _janela.AddMember("largura", 800, allocator);
        _janela.AddMember("altura",  460, allocator);
        _janela.AddMember("titulo",  rapidjson::Value(nome, allocator), allocator);
        _janela.AddMember("icone",   rapidjson::Value("icon.ico", allocator), allocator);

        newDoc.AddMember("nome", rapidjson::Value(nome, allocator), allocator);
        newDoc.AddMember("lancamento", rapidjson::Value("/fases/main", allocator), allocator);
        newDoc.AddMember("janela", _janela, allocator);
        
        // Salvar o JSON no arquivo
        std::ofstream ofs(diretorioDoProjeto + "/config.json");
        if (ofs.is_open()) {
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            newDoc.Accept(writer);
            ofs << buffer.GetString();
            ofs.close();
        }

        
        // Diretório de fases
        if(std::filesystem::create_directories(diretorioDoProjeto + "/fases"))
        {
            std::ofstream fase_file(diretorioDoProjeto + "/fases/main.fase");
            if(fase_file.is_open())
            {
                fase_file << R"({
    "nome": "FaseMain",
    "entidades":[
        {
            "componentes":[
                {
                    "tipo": "luz_direcional",
                    "direcao": [1, 1, 1],
                    "cor": [1, 1, 1],
                    "ambiente": [0.1, 0.1, 0.1],
                    "intensidade": 1.0
                }
            ]
        },
        {
            "componentes":[
                {
                    "tipo": "renderizador",
                    "modelo": "/cubo"
                },
                {
                    "tipo": "transformacao",
                    "posicao": [0,0,5],
                    "rotacao": [0,0,0],
                    "escala": [1, 1, 1]
                },
                {
                    "tipo": "codigo",
                    "diretorio": "/codigos/rotacao.lua"
                }
            ]
        },
        {
            "componentes":[
                {
                    "tipo": "transformacao",
                    "posicao": [0,0,0],
                    "rotacao": [0,90,0],
                    "escala": [1, 1, 1]
                },
                {
                    "tipo": "camera"
                }
            ]
        }
    ]
})";
                fase_file.close();
            }
    }
    
        // Diretorio de codigos
        if(std::filesystem::create_directories(diretorioDoProjeto + "/codigos"))
        {
            std::ofstream codigo_file(diretorioDoProjeto + "/codigos/rotacao.lua");
            if(codigo_file.is_open())
            {
                codigo_file << R"(
-- Autor Daniel O. Santos copyright 2025
local vel = 6

function iniciar()
end
function atualizar()
    eu.transformacao.rotacao.y = eu.transformacao.rotacao.y + vel * tempo.obterDeltaTime()
    eu.transformacao.rotacao.x = eu.transformacao.rotacao.x + vel * tempo.obterDeltaTime()
end

                )";
                codigo_file.close();
            }
        }
}

rapidjson::Document projeto::obterDoc()
{
    // Torna projeto atual
    projeto_atual = this;

    // procura recursivamente por config.json
    std::string caminhoEncontrado = diretorioDoProjeto;
    
    if(std::filesystem::exists(diretorioDoProjeto))
    for (const auto& entry : std::filesystem::recursive_directory_iterator(diretorioDoProjeto)) {
            if (entry.is_regular_file() && entry.path().filename() == "config.json") {
                caminhoEncontrado = entry.path().parent_path().string();
            }
        }

    std::string full_path = caminhoEncontrado + "/config.json";

    if (!std::filesystem::exists(full_path)) 
    {
        depuracao::emitir(alerta, "Arquivo de projeto não encontrado. Digite um nome para criar um novo projeto:");
        std::string resp;
        std::cin >> resp;

        criarProjetoVazio(diretorioDoProjeto, resp.c_str());

        return obterDoc();
    }

    // Executa o parsing
    std::ifstream file(full_path);
    std::stringstream sb;
    sb << file.rdbuf();
    file.close();

    rapidjson::Document doc;
    doc.Parse(sb.str().c_str());

    diretorioDoProjeto = caminhoEncontrado;

    if (doc.HasParseError()) {
        depuracao::emitir(erro, "Erro ao analisar o JSON de configuração.");
    } else {
        depuracao::emitir(info, "Iniciando projeto " + std::string(doc["nome"].GetString()) + " em: " + diretorioDoProjeto);
    }

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
    iniciarSistemas(fase_atual.get());
}

void projeto::iniciarSistemas(bubble::fase* f)
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

void projeto::adicionar(const std::string nome, sistema* s)
{
    s->inicializar(fase_atual.get());
    sistemas_adicionais[nome] = s;
}

std::shared_ptr<fase> projeto::obterFaseAtual()
{
	return fase_atual;
}

sistema_fisica* projeto::sfisica()
{
    return dynamic_cast<sistema_fisica*>(sistemas["fisica"]);
}
sistema_renderizacao* projeto::srender()
{
    return dynamic_cast<sistema_renderizacao*>(sistemas["render"]);
}
sistema* projeto::obterSistema(const std::string nome)
{
    if(sistemas_adicionais.find(nome) != sistemas_adicionais.end())
    {
        return sistemas_adicionais[nome];
    }
    return nullptr;
}
