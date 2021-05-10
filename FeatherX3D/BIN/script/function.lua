
---ButtonPress---------------------------------------------------------------------------------------------------------------
function AccCoin()
	coin_count = coin_count + 1
	if coin_count == 21 then
		coin_count = 1
		EffectCoin()
	else
		SetImage(money_panel,"image",coin_image[coin_count])
	end
		
end


function EffectCoin()
	DeleteItem(money_panel)
	bCoinEffect = 1
	StartIdle()
end
