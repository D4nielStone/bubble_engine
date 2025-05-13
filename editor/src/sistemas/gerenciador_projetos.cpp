/// @copyright Copyright (c) 2025 Daniel Oliveira */
///
/// @file gerenciador_projetos.cpp
///
/// @author Daniel O. dos Santos
///

#include "becommons/becommons.hpp"
#include "editor_namespace.hpp"
#include "sistemas/editor.hpp"
#include "util/runtime.hpp"
#include "sistemas/gerenciador_projetos.hpp"

using namespace EDITOR_NS;

gerenciador_projetos::gerenciador_projetos(const std::string& Dir) {
    DIR_PADRAO = Dir;
}
void gerenciador_projetos::criarProjetoPadrao(const std::string& novo_diretorio, const char* nome)
{       
    auto fase_string = R"({
        "nome": "FaseMain",
        "entidades":
        [
            {
                "id": 1,
                "componentes":[
                    {
                        "tipo": "luz_direcional",
                        "direcao": [-0.25, -0.75, 1],
                        "cor": [1, 1, 1],
                        "ambiente": [0.1, 0.1, 0.1],
                        "intensidade": 1.0
                    }
                ]
            },
            {
                "id": 2,
                "componentes":[
                    {
                        "tipo": "renderizador",
                        "modelo": "/cubo"
                    },
                    {
                        "tipo": "transformacao",
                        "posicao": [0,-2, 15],
                        "rotacao": [0,0,0],
                        "escala": [10, 0.25, 10]
                    }
                ]
            },
            {
                "id": 3,
                "componentes":[
                    {
                        "tipo": "renderizador",
                        "modelo": "/cubo"
                    },
                    {
                        "tipo": "transformacao",
                        "posicao": [0,0,15],
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
                "id": 4,
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
    auto codigo_string = R"(
    -- Autor Daniel O. Santos copyright 2025
    local vel = 6
    
    function iniciar()
    end
    function atualizar()
        eu.transformacao.rotacao.y = eu.transformacao.rotacao.y + vel * tempo.obterDeltaTime()
        eu.transformacao.rotacao.x = eu.transformacao.rotacao.x + vel * tempo.obterDeltaTime()
    end)";

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
            fase_file << fase_string;
            fase_file.close();
        }
    }
    
    // Diretorio de codigos
    if(std::filesystem::create_directories(diretorioDoProjeto + "/codigos"))
    {
        std::ofstream codigo_file(diretorioDoProjeto + "/codigos/rotacao.lua");
        if(codigo_file.is_open())
        {
            codigo_file << codigo_string;
            codigo_file.close();
        }
    }
}
void gerenciador_projetos::removerProjeto(const std::string& dir) {
    if(std::filesystem::exists(dir)) {
        std::filesystem::remove_all(dir);
    }
    atualizarElementos(DIR_PADRAO);
}
void gerenciador_projetos::abrirProjeto(const std::string& caminho) {
       glfwDestroyWindow(janela::obterInstancia().window);
                
       descarregarShaders();
       // Cria projeto
       becommons::projeto editor(caminho);
       
       // Sistema do editor
       editor.adicionar("editor", new sistema_editor());
       
       
       // Define o nome da janela
       janela::obterInstancia().nome(
               (std::string("editor | Daniel O. dos Santos© Bubble 2025 | ") 
                + janela::obterInstancia().nome()).c_str());
       
       // Inicia main loop
       editor.rodar();
       atualizarElementos(DIR_PADRAO);
}

void gerenciador_projetos::atualizarElementos(const std::string& Dir) {
}


void gerenciador_projetos::configurarUI(const std::string& DIR_PADRAO) {
    // Sistema de gui
    gui.inicializar(nullptr);
    gui.iniciarRaiz("raiz");
        gui.defFlags(flag_estilo::modular);
    gui.fimEstilo();
    // configura interface
    gui.adicionar<caixa>("raiz", "#barra_lateral");
        gui.defFlags(flag_estilo::modular | flag_estilo::largura_justa);
        gui.defOrientacao(estilo::orientacao::vertical);
        gui.defCorFundo({0.1, 0.1, 0.1, 1});
        gui.defPaddingG(5, 20);
        gui.defAltura(1.0);
    gui.fimEstilo();
    
    atualizarElementos(DIR_PADRAO);
    // Area Maior
    gui.adicionar<caixa>("raiz", "#area_maior");
        gui.defFlags(flag_estilo::modular);
        gui.defAltura(1.0);
        gui.defOrientacao(estilo::orientacao::vertical);
        gui.defCrescimentoM(1.0);
    gui.fimEstilo();
    gui.adicionar<caixa>("#area_maior", "##cima");
        gui.defOrientacao(estilo::orientacao::vertical);
        gui.defFlags(flag_estilo::modular | flag_estilo::altura_justa);
        gui.defLargura      (1.0);
        gui.defCorFundo({0.2, 0.2, 0.2, 1});
    gui.fimEstilo();
    gui.adicionar<caixa>("#area_maior", "##baixo");
        gui.defFlags(flag_estilo::modular | flag_estilo::alinhamento_central);
        gui.defLargura      (1.0);
        gui.defCorFundo({0.21, 0.21, 0.21, 1});
        gui.defCrescimentoM (1.0);
    gui.fimEstilo();
    gui.adicionar<elementos::texto>("##cima", "###texto1", "Projeto selecionado:", 20);
    gui.adicionar<elementos::texto>("##cima", "###texto2", &m_projeto_selecionado, 15);
    gui.defPadding(5, 5);
    gui.fimEstilo();
}

void gerenciador_projetos::iniciar() {
    if(!std::filesystem::exists(DIR_PADRAO))
        {
            depuracao::emitir(alerta, "Diretório do projeto inexistente.");
            if(!std::filesystem::create_directories(DIR_PADRAO))
            {
                depuracao::emitir(erro, "criando dir padrao.");
                return;
            }
        }
    janela::gerarInstancia("gerenciador de projetos | Daniel O. dos Santos© Bubble 2025", true);
   
    configurarUI(DIR_PADRAO);

    while(!glfwWindowShouldClose(janela::obterInstancia().window))
    {
        janela::obterInstancia().poll();
        
        gui.atualizar();

        janela::obterInstancia().swap();
    }
}
