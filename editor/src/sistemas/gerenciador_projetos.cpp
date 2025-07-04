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
 * LIABILITY, WHETHER IN AN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "becommons/becommons.hpp"
#include "editor_namespace.hpp"
#include "sistemas/editor.hpp"
#include "sistemas/gerenciador_projetos.hpp"

using namespace EDITOR_NS;

/**
 * @brief Construtor da classe gerenciador_projetos.
 * Inicializa o diretório raiz padrão onde os projetos serão armazenados.
 * @param dir O caminho do diretório raiz padrão.
 */
gerenciador_projetos::gerenciador_projetos(const std::string& dir) : DIR_PADRAO(dir) {
}

/**
 * @brief Cria um novo projeto com uma estrutura inicial.
 * Este método gera o diretório do projeto, um arquivo de configuração `config.json`,
 * uma fase padrão `main.fase` com entidades predefinidas (luz, plano, cubo com script)
 * e um script Lua de exemplo.
 *
 * @param novo_diretorio O diretório pai onde o novo projeto será criado.
 * @param nome O nome do novo projeto.
 * @param padrao Flag para criar estrutura padrão.
 */
void gerenciador_projetos::criarProjeto(const std::string& novo_diretorio, const char* nome, const bool padrao) {
    // String JSON que define a fase inicial do projeto com entidades predefinidas.
    // Inclui uma luz direcional, um plano (cubo escalado) e um cubo com um script de rotação.
    auto fase_string = 
    R"({
    "nome": "FaseMain",
    "entidades":
    []
    })";
    if(padrao) fase_string =
    R"({
    "nome": "FaseMain",
    "entidades":
    [
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
                    "tipo": "camera",
                    "skybox": true
                }
            ]
        }
    ]
})";
    // String Lua para o script de rotação de exemplo.
    auto codigo_string = R"(-- Autor Daniel O. © 2025
local vel= 6
local eu = entidade(bubble.meuID)
function atualizar()
    eu.transformacao.rotacao.y = eu.transformacao.rotacao.y + vel
    eu.transformacao.rotacao.x = eu.transformacao.rotacao.x + vel
    eu.transformacao.rotacao.z = eu.transformacao.rotacao.z + vel
end)";

    // Cria o diretório raiz do novo projeto (e seus pais, se necessário).
    std::filesystem::create_directories(novo_diretorio + "/" + nome);

    std::string diretorioDoProjeto = novo_diretorio + "/" + nome;

    // Cria um novo documento JSON para a configuração do projeto.
    rapidjson::Document newDoc;
    newDoc.SetObject();
    rapidjson::Document::AllocatorType& allocator = newDoc.GetAllocator();

    // Adiciona as propriedades da janela ao JSON de configuração.
    rapidjson::Value _janela(rapidjson::kObjectType);
    _janela.AddMember("largura", 640, allocator);
    _janela.AddMember("altura",  480, allocator);
    _janela.AddMember("titulo",  rapidjson::Value(nome, allocator), allocator);
    _janela.AddMember("icone",   rapidjson::Value("scene.png", allocator), allocator);

    // Adiciona as propriedades gerais do projeto ao JSON de configuração.
    newDoc.AddMember("nome", rapidjson::Value(nome, allocator), allocator);
    newDoc.AddMember("lancamento", rapidjson::Value("/fases/main", allocator), allocator);
    newDoc.AddMember("janela", _janela, allocator);

    // Salva o JSON de configuração no arquivo 'config.json' dentro do diretório do projeto.
    std::ofstream ofs(diretorioDoProjeto + "/config.json");
    if (ofs.is_open()) {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        newDoc.Accept(writer);
        ofs << buffer.GetString();
        ofs.close();
    }

    // Cria o diretório 'fases' e escreve o conteúdo da fase padrão 'main.fase'.
    if(std::filesystem::create_directories(diretorioDoProjeto + "/fases"))
    {
        std::ofstream fase_file(diretorioDoProjeto + "/fases/main.fase");
        if(fase_file.is_open())
        {
            fase_file << fase_string;
            fase_file.close();
        }
    }

    // Cria o diretório 'codigos' e escreve o script Lua de exemplo 'rotacao.lua'.
    if(std::filesystem::create_directories(diretorioDoProjeto + "/codigos"))
    {
        std::ofstream codigo_file(diretorioDoProjeto + "/codigos/rotacao.lua");
        if(codigo_file.is_open())
        {
            codigo_file << codigo_string;
            codigo_file.close();
        }
    }

    // Atualiza a lista de projetos disponíveis após a criação do novo projeto.
    buscarProjetos();
}

/**
 * @brief Remove um projeto existente.
 * Exclui recursivamente o diretório do projeto e atualiza a lista de projetos.
 * O projeto selecionado é redefinido para "nenhum" ou para o primeiro projeto disponível.
 * @param dir O caminho completo do diretório do projeto a ser removido.
 */
void gerenciador_projetos::removerProjeto(const std::string& dir) {
    if(std::filesystem::exists(dir)) {
        std::filesystem::remove_all(dir); // Remove o diretório e todo o seu conteúdo
    }

    buscarProjetos(); // Atualiza a lista de projetos
    // Define o projeto selecionado para 'nenhum' se não houver mais projetos,
    // ou para o primeiro projeto na lista.
    m_projeto_selecionado = projetos.empty() ? "nenhum" : projetos.begin()->first;
}

/**
 * @brief Abre e inicia um projeto no editor.
 * Esta função descarrega recursos existentes, destrói a janela atual (do gerenciador),
 * inicializa uma nova instância do projeto e do sistema do editor, configura o título da janela,
 * e inicia o loop principal do editor.
 * @param caminho O caminho completo para o diretório do projeto a ser aberto.
 */
void gerenciador_projetos::abrirProjeto(const std::string& caminho) {
    // Limpeza e preparação para iniciar o editor.
    descarregarShaders();
    imageLoader::init();
    glfwDestroyWindow(janela::obterInstancia().window);

    // Cria uma nova instância do projeto e do sistema do editor.
    std::string caminho_ = caminho;
    becommons::projeto editor(caminho_); // Carrega o projeto
    sistema_editor* e = new sistema_editor(); // Cria o sistema do editor
    e->inicializar(); // Inicializa o sistema do editor
    editor.sistemas.push_back(e); // Adiciona o sistema do editor ao projeto

    // Define o título da janela para indicar que o editor está ativo e qual projeto está aberto.
    janela::obterInstancia().nome(
        (std::string("Bubble::Editor © 2025 - projeto::")
        + janela::obterInstancia().nome()).c_str());

    // Inicia o main loop do projeto/editor.
    editor.rodar();
}

/**
 * @brief Configura a interface do usuário (UI) do gerenciador de projetos.
 * Define a estrutura visual do gerenciador, incluindo a barra lateral para listagem de projetos
 * e a área principal para criação, abertura e remoção de projetos.
 */
void gerenciador_projetos::configurarUI() {
    // Configura a caixa raiz da UI.
    auto& raiz = ui.m_raiz;

    // Configura a barra lateral esquerda para listar os projetos.
    barra_lateral = raiz->adicionar<caixa>();
    barra_lateral->m_estilo.m_flag_estilo |= flag_estilo::altura_percentual;
    barra_lateral->m_estilo.m_cor_borda = {0.05f, 0.05f, 0.05f, 1.f};
    barra_lateral->m_estilo.m_cor_fundo = {0.1f, 0.1f, 0.1f, 1.f};
    barra_lateral->m_estilo.m_padding_geral = {5, 1};
    barra_lateral->m_estilo.m_altura = 1.0;
    barra_lateral->m_estilo.m_largura = 200;

    buscarProjetos(); // Popula a barra lateral com os projetos encontrados.

    // Configura a área principal à direita da barra lateral.
    auto* area_maior = raiz->adicionar<caixa>();
    area_maior->m_estilo.m_flag_estilo |= flag_estilo::altura_percentual | flag_estilo::largura_percentual;
    area_maior->m_estilo.m_altura = 1;
    area_maior->m_estilo.m_largura = 1;
    area_maior->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
    area_maior->m_estilo.m_cor_fundo = cor(0.21, 0.21, 0.21, 1);
    
    // Seção superior da área principal, exibindo o projeto selecionado.
    auto* cima = area_maior->adicionar<caixa>();
    cima->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_justa;
    cima->m_estilo.m_largura = 1;
    cima->m_estilo.m_altura = 80;
    cima->m_estilo.m_cor_fundo = {0.1, 0.1, 0.1, 1};
    cima->m_estilo.m_padding_geral = {5, 5};
    cima->adicionar<elementos::texto>("Projetos Selecionado: ", cor(1.f), 20);
    cima->adicionar<elementos::texto>(&m_projeto_selecionado); // Exibe o nome do projeto selecionado.

    // Seção do meio da área principal, com controles para gerenciar projetos.
    auto* meio = area_maior->adicionar<caixa>();
    meio->m_estilo.m_flag_estilo |= flag_estilo::alinhamento_central | flag_estilo::largura_percentual | flag_estilo::altura_justa;
    meio->m_estilo.m_largura = 1;
    meio->m_estilo.m_padding_geral = ivet2(5, 5);
    meio->m_estilo.m_cor_fundo = cor(0.18f, 0.18f, 0.185f, 1.f);

    // Estilo padrão para os botões de ação.
    estilo e;
    e.m_padding_geral = {5, 0};

    // Botões para adicionar, abrir e remover projetos.
    auto* btn_add = meio->adicionar<elementos::botao>(nullptr, std::make_unique<elementos::imagem>("adicionar.png", false, 1));
    auto* add_popup = btn_add->adicionar<elementos::popup>();

    // Caixa de texto para inserir o nome de um novo projeto.
    auto* caixa_texto = add_popup->adicionar<elementos::caixa_de_texto>("Digite o nome do projeto aqui...", &buffer_projeto);
    caixa_texto->m_estilo.m_cor_fundo = cor(0.12f, 0.12f, 0.12f, 1.f);
    caixa_texto->m_estilo.m_cor_borda = cor(0.1f, 0.1f, 0.1f, 1.f);
    btn_add->m_estilo = e;
    add_popup->adicionar<elementos::botao>([this]() {
            if (!buffer_projeto.empty()) criarProjeto(DIR_PADRAO, buffer_projeto.c_str(), false);
        }, "Projeto Vazio");
    add_popup->adicionar<elementos::botao>([this]() {
            if (!buffer_projeto.empty()) criarProjeto(DIR_PADRAO, buffer_projeto.c_str(), true);
        }, "Projeto Padrao");

    meio->adicionar<elementos::botao>([this]() {
            if(m_projeto_selecionado != "nenhum") {
                abrirProjeto(projetos[m_projeto_selecionado]);
            }
        }, std::make_unique<elementos::imagem>("abrir.png", false, 1))->m_estilo = e;
    meio->adicionar<elementos::botao>([this]() {
            if(m_projeto_selecionado != "nenhum") {
                removerProjeto(projetos[m_projeto_selecionado]);
            }
        }, std::make_unique<elementos::imagem>("remover.png", false, 1))->m_estilo = e;
}

/**
 * @brief Inicia o gerenciador de projetos.
 * Este é o ponto de entrada principal para a aplicação do gerenciador.
 * Inicializa recursos gráficos, verifica e cria o diretório padrão de projetos,
 * configura a janela e a UI, e inicia o loop de eventos principal.
 */
void gerenciador_projetos::iniciar() {
    imageLoader::init(); // Inicializa o carregador de imagens.

    // Verifica se o diretório padrão de projetos existe; se não, tenta criá-lo.
    if(!std::filesystem::exists(DIR_PADRAO))
    {
        depuracao::emitir(alerta, "Diretório do projeto inexistente.");
        if(!std::filesystem::create_directories(DIR_PADRAO))
        {
            depuracao::emitir(erro, "criando dir padrao.");
            return; // Retorna se não for possível criar o diretório.
        }
    }
    // Gera a instância da janela para o gerenciador de projetos.
    janela::gerarInstancia("gerenciador de projetos | Daniel O. dos Santos© Bubble 2025", false, {640, 480}, "folder.png");

    configurarUI(); // Configura a interface gráfica do gerenciador.
    ui.inicializar(); // Inicializa o sistema de UI.

    // Loop principal do gerenciador de projetos.
    // Continua executando enquanto a janela não for solicitada a fechar.
    while(!glfwWindowShouldClose(janela::obterInstancia().window))
    {
        janela::obterInstancia().poll(); // Processa eventos da janela.
        ui.atualizar(); // Atualiza a interface do usuário.
        janela::obterInstancia().swap(); // Troca os buffers da janela para renderizar.
    }
}

/**
 * @brief Busca e lista todos os projetos existentes no diretório padrão.
 * Popula o mapa `projetos` com os nomes e caminhos dos projetos encontrados.
 * Se a barra lateral da UI estiver disponível, atualiza dinamicamente os botões
 * para cada projeto encontrado.
 */
void gerenciador_projetos::buscarProjetos() {
    projetos.clear(); // Limpa a lista de projetos anteriores.
    // Itera sobre os subdiretórios no DIR_PADRAO para encontrar projetos.
    if(std::filesystem::exists(DIR_PADRAO))
    for (const auto& entry : std::filesystem::directory_iterator(DIR_PADRAO)) {
        if (entry.is_directory()) {
            auto dir = entry.path().string();
            // Tenta analisar o arquivo de configuração do projeto.
            auto doc = projeto::analisarProjeto(dir);
            if(!doc.HasMember("nome")) throw std::runtime_error("Analisando projetos do gerenciador de projetos: Projeto sem nome.");
            std::string nome = doc["nome"].GetString();
            projetos[nome] = dir; // Armazena o nome e o diretório do projeto.
            depuracao::emitir(info, "gerenciador_projetos", std::string("Projeto encontrado: ") + nome);
        }
    }
    // Atualiza a barra lateral da UI com os projetos encontrados, se a barra existir.
    if(barra_lateral) {
        barra_lateral->m_filhos.clear(); // Limpa os botões de projetos existentes.
        auto* txt = barra_lateral->adicionar<elementos::texto>("Projetos encontrados:");
        txt->m_estilo.m_flag_estilo |= flag_estilo::quebrar_linha;
        // Adiciona um botão para cada projeto, permitindo a seleção.
        for(auto& [nome, diretorio] : projetos) {
            auto* btn = barra_lateral->adicionar<elementos::botao>([nome, this]() {
                m_projeto_selecionado = nome; // Define o projeto selecionado ao clicar.
                }, nome, "joystick.png"); // Usa um ícone de joystick para projetos.
            btn->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::quebrar_linha;
            btn->m_estilo.m_cor_borda = cor(0.11f);
            btn->m_estilo.m_largura = 2;
            btn->m_estilo.m_padding_geral = {5, 0};
        }
    }
}
