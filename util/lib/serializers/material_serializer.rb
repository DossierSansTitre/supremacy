class MaterialSerializer < Serializer
  type :material

  def serialize(material)
    color = material.color('Color', '#000000')
    background_color = material.color('BackgroundColor', '#030303')
    drop_item = material.ref(:item, 'DropItem', 0)
    floor_symbol = material.symbol('FloorSymbol', 0)
    minable = material.boolean('Minable') ? 1 : 0

    emit(material.id, 'S<')
    emit(color, 'C3')
    emit(background_color, 'C3')
    emit(drop_item, 'S<')
    emit(floor_symbol, 'S<')
    emit(minable, 'C')
  end
end
