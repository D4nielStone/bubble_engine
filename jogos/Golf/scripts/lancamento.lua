local cubo
local cam
local plano
local gatilho = false
local contando_forca = false
local forca = 100
local meio

-- Obtém o diretório do próprio script
local script_path = debug.getinfo(1, "S").source:sub(2):match("(.*/)")
package.path = package.path .. ";" .. script_path .. "?.lua"
local modulo = require("arrastando")
local gamestate = require("gamestate")

function iniciar()
    cubo = entidade(10) -- Criar o cubo
    cam = entidade(5)   -- Criar a câmera
    plano = entidade(11)   -- Criar o plano
    meio = entidade(3).imagem
    cubo.transformacao:apontarEntidade(eu.id)
end

function atualizar()
    gamestate.load()
    local deltaTime = tempo.obterDeltaTime()
    local tamanhoTela = inputs:tamanhoTela()
    local meioTela = vetor2d(tamanhoTela.x / 2, tamanhoTela.y / 2)

    if inputs:mouse().x > 0 and inputs:mouse().y > 0 then

        -- Sistema de lançamento
        local raio_circulo = tamanhoTela.y / 12

        plano.transformacao.posicao.y = eu.transformacao.posicao.y

        -- Converter a posição do mouse para a posição no mundo
        local mousePosTela = inputs:mouse()

        -- Obter origem e direção do raio
        local raio = cam.camera:pontoParaRaio(inputs:mouse())
        local resultado = fisica.raioIntersecta(raio, plano.fisica:corpoRigido())

        if resultado.atingiu then
            local direcao = util.normalizarV3(vetor3(
                resultado.pontoDeColisao.x - eu.transformacao.posicao.x,
                resultado.pontoDeColisao.y - eu.transformacao.posicao.y,
                resultado.pontoDeColisao.z - eu.transformacao.posicao.z
            ))

            cubo.transformacao.posicao = vetor3(
                (direcao.x + eu.transformacao.posicao.x),
                eu.transformacao.posicao.y,
                (direcao.z + eu.transformacao.posicao.z)
            )

            local arrastando = modulo.arrastando(raio_circulo)

            local distancia_ = util.distanciaV3(resultado.pontoDeColisao, eu.transformacao.posicao)
            -- Definindo velocidade com ajuste de deltaTime
            local velocidadetotal = eu.fisica:obtVelocidade().x +
                                    eu.fisica:obtVelocidade().y +
                                    eu.fisica:obtVelocidade().z 

            if contando_forca and not inputs.pressionada("MouseE") and velocidadetotal < 0.1 and not gamestate.lost then
                eu.fisica:defVelocidade(vetor3(
                    direcao.x * -(distancia_ * forca * deltaTime),
                    0,
                    direcao.z * -(distancia_ * forca * deltaTime)
                ))
                gamestate.setTrys(gamestate.trys - 1)
                gamestate.write()
            end

            -- O arrasto serve para detectar se o mouse está 
            -- arrastando um círculo no centro da tela para o lançamento
            local meioViewport = vetor2d(meio.limite.x / 2 + meio.padding.x, meio.limite.y / 2 + meio.padding.y)
            if arrastando and util.distanciaV2(inputs:mouse(), meioViewport) > raio_circulo then
                contando_forca = true
            else
                contando_forca = false
            end
        end

        -- Aparição cubos
        if contando_forca then
            cubo.transformacao.escala.x = 0.15
            cubo.transformacao.escala.y = 0.15
            cubo.transformacao.escala.z = 0.15
        else
            cubo.transformacao.escala.x = 0
            cubo.transformacao.escala.y = 0
            cubo.transformacao.escala.z = 0
        end
    end
end
