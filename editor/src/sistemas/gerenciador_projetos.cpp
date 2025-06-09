/** \copyright 
 * MIT License
 * Copyright (c) 2025 Daniel Oliveira
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 */

#include "becommons/becommons.hpp"
#include "editor_namespace.hpp"
#include "sistemas/editor.hpp"
#include "sistemas/gerenciador_projetos.hpp"

using namespace EDITOR_NS;

gerenciador_projetos::gerenciador_projetos(const std::string& dir) : DIR_PADRAO(dir) {
}
void gerenciador_projetos::criarProjetoPadrao(const std::string& novo_diretorio, const char* nome) {       
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
-- Autor Daniel O. Santos copyright 2024
local vel = 6

function iniciar()
end
function atualizar()
    eu.transformacao.rotacao = fvet3(eu.transformacao.rotacao.x + vel * tempo.obterDeltaTime(), eu.transformacao.rotacao.y + vel * tempo.obterDeltaTime(), 0)
end
    )";

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

    buscarProjetos();
}

void gerenciador_projetos::removerProjeto(const std::string& dir) {
    if(std::filesystem::exists(dir)) {
        std::filesystem::remove_all(dir);
    }

    buscarProjetos();
    m_projeto_selecionado = projetos.empty() ? "nenhum" : projetos.begin()->first;
}
void gerenciador_projetos::abrirProjeto(const std::string& caminho) {
       glfwDestroyWindow(janela::obterInstancia().window);
                
       std::string caminho_ = caminho;
       descarregarShaders();
       // Cria projeto
       becommons::projeto editor(caminho_);
       
       // Sistema do editor
       editor.adicionar("editor", new sistema_editor());
       
       
       // Define o nome da janela
       janela::obterInstancia().nome(
               (std::string("editor | Daniel O. dos Santos© Bubble 2025 | ") 
                + janela::obterInstancia().nome()).c_str());
       
       // Inicia main loop
       editor.rodar();
}


void gerenciador_projetos::configurarUI() {
    // configura interface
    auto& raiz = ui.m_raiz;
    barra_lateral = raiz->adicionar<caixa>();
    barra_lateral->m_estilo.m_flag_estilo |= flag_estilo::altura_percentual;
    barra_lateral->m_estilo.m_cor_borda = {0.05f, 0.05f, 0.05f, 1.f};
    barra_lateral->m_estilo.m_cor_fundo = {0.1f, 0.1f, 0.1f, 1.f};
    barra_lateral->m_estilo.m_padding_geral = {5, 1};
    barra_lateral->m_estilo.m_altura = 1.0;
    barra_lateral->m_estilo.m_largura = 200;
    
    buscarProjetos();

    // area maior
    auto* area_maior = raiz->adicionar<caixa>();
    area_maior->m_estilo.m_flag_estilo |= flag_estilo::altura_percentual | flag_estilo::largura_percentual;
    area_maior->m_estilo.m_altura = 1;
    area_maior->m_estilo.m_largura = 1;
    area_maior->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
    area_maior->m_estilo.m_cor_fundo = cor(0.21, 0.21, 0.21, 1);
    // cima
    auto* cima = area_maior->adicionar<caixa>();
    cima->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_justa;
    cima->m_estilo.m_largura = 1;
    cima->m_estilo.m_altura = 80;
    cima->m_estilo.m_cor_fundo = {0.1, 0.1, 0.1, 1};
    cima->m_estilo.m_padding_geral = {5, 5};
    cima->adicionar<elementos::texto>("Projetos Selecionado: ", cor(1.f), 20);
    cima->adicionar<elementos::texto>(&m_projeto_selecionado);
    // meio
    auto* meio = area_maior->adicionar<caixa>();
    meio->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_justa;
    meio->m_estilo.m_largura = 1;
    meio->m_estilo.m_padding_geral = ivet2(5, 5);
    meio->m_estilo.m_cor_fundo = cor(0.18f, 0.18f, 0.185f, 1.f);

    // caixa texto
    auto* caixa_texto = meio->adicionar<elementos::caixa_de_texto>("Digite o nome do projeto aqui...", &buffer_projeto);
    caixa_texto->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::quebrar_linha;
    caixa_texto->m_estilo.m_largura = 1;
    caixa_texto->m_estilo.m_cor_fundo = cor(0.12f, 0.12f, 0.12f, 1.f);
    caixa_texto->m_estilo.m_cor_borda = cor(0.1f, 0.1f, 0.1f, 1.f);
    
    meio->adicionar<elementos::botao>([this]() {
            if (!buffer_projeto.empty()) criarProjetoPadrao(DIR_PADRAO, buffer_projeto.c_str());
        }, " adicionar ", "adicionar.png")->m_estilo.m_cor_borda = cor(0.3f);
    meio->adicionar<elementos::botao>([this]() {
            if(m_projeto_selecionado != "nenhum") {
                abrirProjeto(projetos[m_projeto_selecionado]);
            }
        }, " abrir ", "abrir.png")->m_estilo.m_cor_borda = cor(0.3f);
    meio->adicionar<elementos::botao>([this]() {
            if(m_projeto_selecionado != "nenhum") {
                removerProjeto(projetos[m_projeto_selecionado]);
            }
        }, " remover ", "remover.png")->m_estilo.m_cor_borda = cor(0.3f);
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
   
    ui.inicializar();
    configurarUI();

    while(!glfwWindowShouldClose(janela::obterInstancia().window))
    {
        janela::obterInstancia().poll();
        
        ui.atualizar();

        janela::obterInstancia().swap();
    }
}

void gerenciador_projetos::buscarProjetos() {
    projetos.clear();
    if(std::filesystem::exists(DIR_PADRAO))
    for (const auto& entry : std::filesystem::directory_iterator(DIR_PADRAO)) {
        if (entry.is_directory()) {
            auto dir = entry.path().string();
            auto doc = projeto::analisarProjeto(dir);
            if(!doc.HasMember("nome")) throw std::runtime_error("Alanisando projetos do gerenciador de projetos.");
            std::string nome = doc["nome"].GetString();
            projetos[nome] = dir;
            depuracao::emitir(info, "gerenciador_projetos", std::string("Projeto encontrados: ") + nome);
        }
    }
    if(barra_lateral) {
    barra_lateral->m_filhos.clear();
    auto* txt = barra_lateral->adicionar<elementos::texto>("Projetos encontrados:");
    txt->m_estilo.m_flag_estilo |= flag_estilo::quebrar_linha;
    for(auto& [nome, diretorio] : projetos) {
        auto* btn = barra_lateral->adicionar<elementos::botao>([nome, this]() {
                m_projeto_selecionado = nome;
                }, " " + nome + " ", "folder.png");
        btn->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::quebrar_linha;
        btn->m_estilo.m_cor_borda = cor(0.3f);
        btn->m_estilo.m_largura = 1;
        btn->m_estilo.m_padding_geral = {5, 0};
    }
    }
}
