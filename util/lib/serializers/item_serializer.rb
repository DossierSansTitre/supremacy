class ItemSerializer < Serializer
  type :item

  def serialize(item)
    symbol = item.fetch('Symbol', ' ').ord
    color = item.color('Color', '#000000')
    [item.id, symbol, color].flatten.pack('S<2C3')
  end
end
