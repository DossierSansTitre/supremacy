class ItemSerializer < Serializer
  type :item

  def serialize(item)
    symbol = item.fetch('Symbol', ' ').ord
    color = item.color('Color', '#000000')

    emit(item.id, 'S<')
    emit(symbol, 'S<')
    emit(color, 'C3')
  end
end
