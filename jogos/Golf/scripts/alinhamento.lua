--[[
	Copyright (c) 2025 Daniel Oliveira dos Santos. Todos os direitos reservados.
	Este código é parte do projeto BubbleEngine.
]]
local s = 0
local s_click 
local img_click
local gatilho = false
local meio
local elapsedTime

local script_path = debug.getinfo(1, "S").source:sub(2):match("(.*/)")
package.path = package.path .. ";" .. script_path .. "?.lua"
local gs = require("gamestate")

function iniciar()
	eu.imagem.padding = vetor2d(inputs:tamanhoTela().x/2-s/2,inputs:tamanhoTela().y/2-s/2)
	eu.imagem.limite = vetor2d(s, s)
    meio = entidade(3).imagem
	img_click = entidade(6)
end

function atualizar()
	s = inputs:tamanhoTela().y / 6

	gs:load()
	if inputs.pressionada("MouseE") or gs.won or gs.lost then
		gatilho = true
		eu.imagem.limite = vetor2d(0, 0)
	else
		local l = util.lerp(eu.imagem.limite.x, s, 0.3)
		eu.imagem.limite = vetor2d(l, l)
		eu.imagem.padding = vetor2d
		(
			meio.padding.x + meio.limite.x/2 - eu.imagem.limite.x/2,
			meio.padding.y + meio.limite.y/2 - eu.imagem.limite.y/2
			)
	end










	s_click = inputs:tamanhoTela().x / 12

	if gatilho then

		img_click.imagem.limite = vetor2d(
        util.lerp(img_click.imagem.limite.x,  5, 0.3),
        util.lerp(img_click.imagem.limite.y, 0.1, 0.3)
    )
    img_click.imagem.padding = vetor2d(inputs:tamanhoTela().x / 2 - img_click.imagem.limite.x / 2, inputs:tamanhoTela().y /8)
    
else

    img_click.imagem.padding = vetor2d(inputs:tamanhoTela().x / 2 - s_click / 2, inputs:tamanhoTela().y /8)
    img_click.imagem.limite = vetor2d(s_click, s_click)
end

end