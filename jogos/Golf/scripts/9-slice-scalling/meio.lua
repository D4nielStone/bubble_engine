--[[
	Copyright (c) 2025 Daniel Oliveira dos Santos. Todos os direitos reservados.
	Este código é parte do projeto BubbleEngine.
]]

-- Obtém o diretório do próprio script
local script_path = debug.getinfo(1, "S").source:sub(2):match("(.*/)")
package.path = package.path .. ";" .. script_path .. "?.lua"

-- Carrega o arquivo de configuração
local config = require("config")

-- Obtém o diretório pai do script
local parent_path = script_path:match("(.*/).*/")
package.path = package.path .. ";" .. parent_path .. "?.lua"

-- Carrega o estado do jogo
local gamestate = require("gamestate")

-- Calcula o valor de 's' baseado na configuração
local s = config.padding + config.escalaBorda

-- Função de inicialização
function iniciar()
	eu.imagem.limite = vetor2d(1, 0)
	eu.imagem.padding = vetor2d(inputs:tamanhoTela().x / 2, inputs:tamanhoTela().y / 2)
end

-- Função de atualização
function atualizar()
	gamestate.load()
	eu.imagem:definirID(entidade(5).camera.framebuffer)
	eu.imagem.padding = vetor2d(inputs:tamanhoTela().x / 2 - eu.imagem.limite.x / 2, s)
	
	-- Verifica se o jogo foi ganho
	if gamestate.won then
		eu.imagem.limite.x = util.lerp(eu.imagem.limite.x, 0, 0.2)
	else
		eu.imagem.limite.x = util.lerp(eu.imagem.limite.x, inputs:tamanhoTela().x - config.padding * 2 - config.escalaBorda * 2, 0.2)
	end
	
	eu.imagem.limite.y = inputs:tamanhoTela().y - config.padding - config.escalaCima * 2
	entidade(5).camera:viewport(vetor2d(eu.imagem.limite.x + eu.imagem.padding.x * 2, eu.imagem.limite.y + eu.imagem.padding.y * 2))
	
	-- Verifica se a imagem limite é menor que 0.5
	if eu.imagem.limite.x < 0.5 then
		gamestate.level = gamestate.level + 1
		gamestate.won = false
		gamestate.lost = false
		gamestate.write()
		projetoAtual:abrirFase("level" .. tostring(gamestate.level))
	end
end