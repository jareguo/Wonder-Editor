/* open FileType;

type state = {
  inputField: ref(option(Dom.element)),
  inputValue: string,
  primitiveName: string,
  postfix: string
};

type action =
  | Blur
  | Change(string);

let setInputFiledRef = (value, {ReasonReact.state}) => state.inputField := Js.Null.to_opt(value);

module Method = {
  let change = (event) => {
    let inputVal = ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value;
    Change(inputVal)
  };
  let blur = (_event) => Blur;
  let triggerBlur = (dispatch, value, fileId, fileResult) => {
    AssetEditorService.setNodeMap(
      StateEditorService.getState()
      |> AssetEditorService.unsafeGetNodeMap
      |> SparseMapService.immutableSet(fileId, {...fileResult, name: value})
    )
    |> StateLogicService.getAndSetEditorState;
    dispatch(AppStore.ReLoad)
  };
  let showFileInfo = (fileResult, {state, handle, reduce}: ReasonReact.self('a, 'b, 'c)) =>
    switch fileResult.type_ {
    | Image =>
      <div className="">
        <h1> (DomHelper.textEl("Image")) </h1>
        <hr />
        <span className=""> (DomHelper.textEl("name:")) </span>
        <input
          ref=(handle(setInputFiledRef))
          className="input-component float-input"
          _type="text"
          value=state.inputValue
          onChange=(reduce(change))
          onBlur=(reduce(blur))
        />
      </div>
    | Json =>
      <div>
        <h1> (DomHelper.textEl("Json")) </h1>
        <hr />
        <span className=""> (DomHelper.textEl("name:")) </span>
        <input
          ref=(handle(setInputFiledRef))
          className="input-component float-input"
          _type="text"
          value=state.inputValue
          onChange=(reduce(change))
          onBlur=(reduce(blur))
        />
        <p> (DomHelper.textEl(fileResult.result |> Js.Option.getExn)) </p>
      </div>
    };
};

let component = ReasonReact.reducerComponent("AssetFileInspector");

let reducer = (dispatch, fileId, fileResult, action, state) =>
  switch action {
  | Change(value) => ReasonReact.Update({...state, inputValue: value})
  | Blur =>
    switch state.inputValue {
    | "" => ReasonReact.Update({...state, inputValue: state.primitiveName})
    | value =>
      ReasonReact.UpdateWithSideEffects(
        {...state, primitiveName: value},
        ((_slef) => Method.triggerBlur(dispatch, value ++ state.postfix, fileId, fileResult))
      )
    }
  };

let render = (fileResult, self) =>
  <article key="AssetFileInspector" className="inspector-component">
    (Method.showFileInfo(fileResult, self))
  </article>;

let make = (~store: AppStore.appState, ~dispatch, ~fileId, ~fileResult: fileResultType, _children) => {
  ...component,
  initialState: () => {
    let (fileName, postfix) = AssetFileInspectorUtils.handleFileName(fileResult.name);
    {inputValue: fileName, primitiveName: fileName, inputField: ref(None), postfix}
  },
  reducer: reducer(dispatch, fileId, fileResult),
  render: (self) => render(fileResult, self)
}; */