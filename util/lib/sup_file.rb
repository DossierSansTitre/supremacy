require 'sup_helper'

class SupFile
  include SupHelper

  attr_reader :type
  attr_reader :id
  attr_reader :name

  @@registry = {}

  def initialize(type)
    @type = type
    @id = nil
    @name = nil
    @data = {}
  end

  def fetch(key, default = nil)
    key.downcase!
    @data[key] || default
  end

  def [](key)
    fetch(key)
  end

  def []=(key, value)
    key.downcase!
    @data[key] = value
    if key == 'id'
      @id = value.to_i
    elsif key == 'name'
      @name = value
    end
    nil
  end

  def register
    @@registry[@type] ||= {}
    @@registry[@type][@name] = @id
    nil
  end

  def self.parse(type, path)
    sup = self.new(type)
    File.open(path, "r") do |file|
      file.each do |line|
        line.chomp!
        line.strip!
        next if line[0] == '#'
        next if line == ''
        key, value = line.split('=', 2).map(&:strip)
        if value.nil?
          value = true
        elsif value[0] == '"' || value[0] == "'"
          value = value[1..-2]
        elsif value =~ /[0-9]+/
          value = value.to_i
        end
        sup[key] = value
      end
    end
    sup.register
    sup
  end

  def self.reference(type, name)
    name.downcase!
    @@registry[type][name]
  end
end
