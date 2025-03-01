-- Obtém o diretório do próprio script
local script_path = debug.getinfo(1, "S").source:sub(2):match("(.*/)")
package.path = package.path .. ";" .. script_path .. "?.lua"
local config = require("config")
local meio
function iniciar()
    meio = entidade(3).imagem
end
function atualizar()
    eu.imagem.limite.x = config.escalaBorda
    eu.imagem.limite.y = meio.limite.y
    eu.imagem.padding.y =  meio.padding.y
    eu.imagem.padding.x = meio.padding.x + meio.limite.x
end