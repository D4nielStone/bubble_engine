-- Obtém o diretório do próprio script
local script_path = debug.getinfo(1, "S").source:sub(2):match("(.*/)")
package.path = package.path .. ";" .. script_path .. "?.lua"
local config = require("config")
local meio
function iniciar()
    meio = entidade(3).imagem
end
function atualizar()
    eu.imagem.limite.x = config.escalaCima
    eu.imagem.limite.y = config.escalaCima
    eu.imagem.padding.y = meio.padding.y + meio.limite.y
    eu.imagem.padding.x = meio.padding.x - eu.imagem.limite.x / 2
end