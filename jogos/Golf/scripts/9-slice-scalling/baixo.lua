--[[
	Copyright (c) 2025 Daniel Oliveira dos Santos. Todos os direitos reservados.
	Este código é parte do projeto BubbleEngine.
]]

-- Obtém o diretório do próprio script
local script_path = debug.getinfo(1, "S").source:sub(2):match("(.*/)")
package.path = package.path .. ";" .. script_path .. "?.lua"
local config = require("config")
local meio
function iniciar()
    meio = entidade(3).imagem
end
function atualizar()
    eu.imagem.limite.x = meio.limite.x
    eu.imagem.limite.y = config.escalaCima
    eu.imagem.padding.y = meio.padding.y - eu.imagem.limite.y/2
    eu.imagem.padding.x = meio.padding.x
end