/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file gerenciador_projetos.cpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#include "becommons/becommons.hpp"
#include "editor_namespace.hpp"
#include "sistemas/editor.hpp"
#include "util/runtime.hpp"
#include "sistemas/gerenciador_projetos.hpp"

using namespace EDITOR_NS;
void gerenciador_projetos::criarProjetoPadrao(const std::string& novo_diretorio, const char* nome)
{
        // Cria diretório do projeto
        std::filesystem::create_directories(novo_diretorio + "/" + nome);
        
        std::string diretorioDoProjeto = novo_diretorio + "/" + nome;
        
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
    atualizarElementos(DIR_PADRAO);
}

gerenciador_projetos::gerenciador_projetos(const std::string& DIR_PADRAO) : DIR_PADRAO(DIR_PADRAO)
{
}

void gerenciador_projetos::atualizarElementos(const std::string& Dir)
{
    gui.removerFilhos("barra_lateral");
    // Barra Lateral (    Texto acima    )
    gui.novoEstilo();
        gui.adicionarElemento<elementos::texto>("barra_lateral", "texto1", "Projetos encontrados", 20, elementos::flags_texto::alinhamento_central);
        gui.defLargura(1.0);
    // Iteração pastas
    gui.novoEstilo();
        for (const auto& entry : std::filesystem::recursive_directory_iterator(Dir)) {
            if (entry.is_regular_file() && entry.path().filename() == "config.json") {
                auto caminhoEncontrado = entry.path().parent_path().string();
                gui.novoEstilo();    
                gui.adicionarElemento<elementos::botao>("barra_lateral", "abrirProjeto", 
                [caminhoEncontrado](){
        
                glfwDestroyWindow(instanciaJanela->window);
                delete instanciaJanela;
                instanciaJanela = nullptr;
                descarregarShaders();
                // Cria projeto
                projeto editor(caminhoEncontrado);
               
                // Sistema do editor
                editor.adicionar("editor", new sistema_editor());
            
            
                // Define o nome da janela
                instanciaJanela->nome(
                        (std::string("editor | Daniel O. dos Santos© Bubble 2025 | ") 
                         + instanciaJanela->nome()).c_str());
            
                // Inicia main loop
                editor.rodar();
        
                }
                , new elementos::imagem("folder.png"));
                gui.defLargura(40);
                gui.defAltura(40);
                gui.novoEstilo();    
                gui.adicionarElemento<elementos::texto>("barra_lateral", caminhoEncontrado, caminhoEncontrado);
            }
        }
}

void gerenciador_projetos::iniciar()
{
    if(!std::filesystem::exists(DIR_PADRAO))
        {
            depuracao::emitir(alerta, "Diretório do projeto inexistente.");
            if(!std::filesystem::create_directories(DIR_PADRAO))
            {
                depuracao::emitir(erro, "criando dir padrao.");
                return;
            }
        }
    instanciaJanela = new janela("gerenciador de projetos | Daniel O. dos Santos© Bubble 2025", true);
   
    // Sistema de gui
    gui.inicializar(nullptr);
    gui.obterElemento("raiz")->m_cor_fundo = {0.18f, 0.18f, 0.18f, 1.f};
    gui.adicionarFlags("raiz", flags_caixa::modular);
    
    // configura interface
    gui.novoEstilo();
    gui.adicionarElemento<caixa>("raiz", "barra_lateral");
        gui.defFlags(flags_caixa::modular | flags_caixa::largura_justa);
        gui.defOrientacao(caixa::orientacao::vertical);
        gui.defCorFundo({0.1, 0.1, 0.1, 1});
        gui.defPaddingG(5, 5);
        gui.defAltura(1.0);
    
    atualizarElementos(DIR_PADRAO);
    // Area Maior
    gui.novoEstilo();
        gui.adicionarElemento<caixa>("raiz", "area_maior");
        gui.defFlags(flags_caixa::modular | flags_caixa::alinhamento_central);
        gui.defAltura(1.0);
        gui.defCrescimentoM(1.0);
    gui.novoEstilo();
        gui.adicionarElemento<elementos::botao>("area_maior", "add", [this](){criarProjetoPadrao(DIR_PADRAO, "Juquinha");std::cout << "AAAAAA";}, new elementos::texto("+", 48));
    while(!glfwWindowShouldClose(instanciaJanela->window))
    {
        instanciaJanela->poll();
        
        gui.atualizar();

        instanciaJanela->swap();
    }
}
