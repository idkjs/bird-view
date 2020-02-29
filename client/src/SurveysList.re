open Ehd;
let str = ReasonReact.string;

let optionStr =
  fun
  | None => ""
  | Some(v) => v;

module SurveysList = {
  [@react.component]
  let make = (~data, ~updatable, ~answerable, ~reportViewable) => {
    <div>
      <Table>
        <thead>
          <TableRow>
            <TableHeader> {"Sent At" |> str} </TableHeader>
            <TableHeader> {"Squad Name" |> str} </TableHeader>
            {updatable
               ? <TableHeader> {"Enabled?" |> str} </TableHeader> : React.null}
            <TableHeader> {"Actions" |> str} </TableHeader>
          </TableRow>
        </thead>
        <tbody>
          {data
           |> List.map(rowData => {
                <TableRow key=rowData##id>
                  <TableData>
                    {rowData##createdAt
                     |> Js.Json.decodeString
                     |> optionStr
                     |> Utils.formatDate
                     |> str}
                  </TableData>
                  <TableData> {rowData##squad##name |> str} </TableData>
                  {updatable
                     ? <TableData>
                         <SurveyStatusSwitch
                           surveyId=rowData##id
                           enabled=rowData##enabled
                         />
                       </TableData>
                     : ReasonReact.null}
                  <TableData>
                    {answerable
                       ? <Route.Link
                           route=Route.Config.(SurveyDetail(rowData##id))>
                           <Button _type=`primary disabled={!rowData##enabled}>
                             {"Answer" |> str}
                           </Button>
                         </Route.Link>
                       : React.null}
                    {reportViewable
                       ? <Route.Link
                           style={ReactDOMRe.Style.make(
                             ~marginLeft="5px",
                             (),
                           )}
                           route=Route.Config.(SurveyReport(rowData##id))>
                           <Button> {"View report" |> str} </Button>
                         </Route.Link>
                       : ReasonReact.null}
                  </TableData>
                </TableRow>
              })
           |> Array.of_list
           |> ReasonReact.array}
        </tbody>
      </Table>
    </div>;
  };
};

module QueryConfig = [%graphql
  {|
    query allSurveys($squadId: ID) {
      allSurveys(filter: { squad: { id: $squadId}}, orderBy: createdAt_DESC) {
        id
        createdAt
        enabled
        squad {
          name
        }
      }
    }
  |}
];

module Query = ReasonApolloHooks.Query.Make(QueryConfig);

[@react.component]
let make = (~squadId, ~role) => {
  switch (squadId) {
  | None =>
    <FriendlyError
      message="You doesn't belong to any squad. Contact Hieu Pham to assign you into a squad!"
    />
  | Some(squadId) =>
    let variables = QueryConfig.make(~squadId, ())##variables;
    let (queryState, _full) = Query.use(~variables, ());

    let updatable =
      switch (role) {
      | Some(`Leader)
      | Some(`Admin) => true
      | _ => false
      };

    let answerable =
      switch (role) {
      | Some(`Leader)
      | Some(`Member) => true
      | _ => false
      };

    let reportViewable =
      switch (role) {
      | Some(`Leader) => true
      | _ => false
      };

    <div>
      <Headline> {str("BIRD VIEW SURVEYS")} </Headline>
      {switch (queryState) {
       | Loading => <Spinner />
       | Data(data) =>
         <div>
           {switch (data##allSurveys |> Belt.Array.size) {
            | 0 => <Ehd.Empty />
            | _ =>
              <SurveysList
                data={data##allSurveys |> Array.to_list}
                updatable
                answerable
                reportViewable
              />
            }}
         </div>
       | NoData => <EmptyData />
       | Error(e) => <FriendlyError message=e##message />
       }}
    </div>;
  };
};
