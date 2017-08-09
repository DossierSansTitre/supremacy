require 'serializer'

class TaskSeriaslizer < Serializer
  type :task

  def serialize(task)
    name = task.fetch('DisplayName', task.name)
    match = task.fetch('Match', '').split(' ').map(&:strip).map{|t| SupFile.reference(:tile, t)}
    into = task.ref(:tile, 'Into', 'Nothing')

    data = [task.id, name.size, name].pack('S<S<a*')
    data += [match.size].pack('S<') + match.pack('S<*')
    data += [into].pack('S<')
    data
  end
end
