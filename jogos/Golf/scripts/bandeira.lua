local bola
local cam
local posantiga
local forcaAtrativa = 20  -- Intensidade da força atrativa_
local _break = false
local meio

local script_path = debug.getinfo(1, "S").source:sub(2):match("(.*/)")
package.path = package.path .. ";" .. script_path .. "?.lua"
local gamestate = require("gamestate")

function iniciar()
    bola = entidade(1)
    meio = entidade(3).imagem
    cam = entidade(5)
    posantiga = eu.transformacao.posicao.y
end

function atualizar()
    if util.distanciaV3(
        vetor3(eu.transformacao.posicao.x, posantiga-1, eu.transformacao.posicao.z), bola.transformacao.posicao
    ) < 0.5
    then 
        if meio.limite.x > 5 then
            gamestate.load()
        gamestate.setWin()
        gamestate.write()
        end
    end
    -- Verifica se a bola está perto da posição desejada para ajustar a altura
    if util.distanciaV2(vetor2d(bola.transformacao.posicao.x, bola.transformacao.posicao.z),
     vetor2d(eu.transformacao.posicao.x, eu.transformacao.posicao.z)) < 3 then
        eu.transformacao.posicao.y = util.lerp(eu.transformacao.posicao.y, posantiga + 4, 0.1)
    else
        eu.transformacao.posicao.y = util.lerp(eu.transformacao.posicao.y, posantiga, 0.1)
    end

    -- Verifica se a bola está perto do buraco
    if util.distanciaV2(vetor2d(bola.transformacao.posicao.x, bola.transformacao.posicao.z),
     vetor2d(eu.transformacao.posicao.x, eu.transformacao.posicao.z)) < 1 then
        -- Aplica uma força de atração
        local direcao = vetor3(eu.transformacao.posicao.x - bola.transformacao.posicao.x, posantiga-1 - bola.transformacao.posicao.y, eu.transformacao.posicao.z - bola.transformacao.posicao.z)
        local direcao_n = util.normalizarV3(direcao)
        bola.fisica:aplicarForca(vetor3(direcao_n.x * forcaAtrativa,direcao_n.y* forcaAtrativa,direcao_n.z* forcaAtrativa))
        
    end
    
end
