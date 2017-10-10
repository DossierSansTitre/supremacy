require 'serializer'

class TaskTerrainSeriaslizer < Serializer
  type :task_terrain

  def serialize(task)
    name = task.fetch('DisplayName', task.name)
    match = task.fetch('Match', '').split(' ').map(&:strip).map{|t| SupFile.reference(:tile, t)}
    into = task.ref(:tile, 'Into', 'Nothing')

    emit(task.id, 'S<')
    emit([name.size, name], 'S<a*')
    emit(match.size, 'S<')
    emit(match, 'S<*')
    emit(into, 'S<')
  end
end
