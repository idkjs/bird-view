[@bs.val] [@bs.scope "localStorage"]
external setItem: (string, option(string)) => unit = "setItem";
[@bs.val] [@bs.scope "localStorage"]
external getItem: string => Js.Nullable.t(string) = "getItem";

[@bs.val] [@bs.scope "localStorage"]
external removeItem: string => unit = "removeItem";
