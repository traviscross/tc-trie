-- generate test data

function get_seed()
  local f=io.open("/dev/urandom","r")
  local x=f:read(4)
  f:close()
  local n=0
  for i=1,4 do
    n=bit32.lshift(n,8)
    n=bit32.bor(n,string.byte(string.sub(x,i,i)))
  end
  return n
end

math.randomseed(get_seed())

function random_tn()
  return math.random(2,9)..string.format("%02d",math.random(0,99))
    ..math.random(2,9)..string.format("%05d",math.random(0,99999))
end

local base64_chars="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/="
function random_char()
  local i=math.random(1,64)
  return string.sub(base64_chars,i,i)
end
function random_provider()
  return random_char()..random_char()
    ..random_char()..random_char()
end

function gen_table(n)
  print("{")
  local out=nil
  for i=1,n do
    if out then print(out..",") end
    out="  \""..random_tn().."\":\""..random_provider().."\""
  end
  if out then print(out) end
  print("}")
end

n=100
if arg[1] then n=tonumber(arg[1]) end
gen_table(n)
