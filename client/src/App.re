[%bs.raw {|require("tailwindcss/dist/tailwind.min.css")|}];

module AppRouter = Router.Create(Route.Config);

let str = ReasonReact.string;

[@react.component]
let make = () =>
  <AppRouter>
    {currentRoute =>
       Route.Config.(
         switch (currentRoute) {
         | Home => <Home />
         | NotFound => <div> {ReasonReact.string("Not Found")} </div>
         }
       )}
  </AppRouter>;
