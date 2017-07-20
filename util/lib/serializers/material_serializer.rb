class MaterialSerializer < Serializer
  type :material

  def serialize(material)
    color = material.color('Color', '#000000')
    background_color = material.color('BackgroundColor', '#030303')
    drop_item = material.ref(:item, 'DropItem', 0)
    floor_symbol = material.symbol('FloorSymbol', 0)
    minable = material.boolean('Minable') ? 1 : 0
    [material.id, color, background_color, drop_item, floor_symbol, minable].flatten.pack('S<C3C3S<S<C')
  end
end
