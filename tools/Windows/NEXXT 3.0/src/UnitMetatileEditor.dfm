object FormMetatileTool: TFormMetatileTool
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Metatile Tool'
  ClientHeight = 451
  ClientWidth = 460
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  KeyPreview = True
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnHide = FormHide
  OnKeyDown = FormKeyDown
  OnKeyPress = FormKeyPress
  OnKeyUp = FormKeyUp
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Image2: TImage
    Left = 4
    Top = 192
    Width = 256
    Height = 256
    OnDragDrop = Image2DragDrop
    OnDragOver = Image2DragOver
    OnEndDrag = Image2EndDrag
    OnMouseDown = Image2MouseDown
    OnMouseEnter = Image2MouseEnter
    OnMouseLeave = Image2MouseLeave
    OnMouseMove = Image2MouseMove
  end
  object Append1: TSpeedButton
    Left = 302
    Top = 409
    Width = 55
    Height = 20
    Caption = 'Append..'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = Append1Click
    OnMouseEnter = Append1MouseEnter
    OnMouseLeave = PageControl1MouseLeave
  end
  object Sort1: TSpeedButton
    Left = 264
    Top = 430
    Width = 45
    Height = 20
    Caption = 'Tidy up..'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = Sort1Click
    OnMouseEnter = Sort1MouseEnter
  end
  object btnMetaClear: TSpeedButton
    Left = 335
    Top = 430
    Width = 32
    Height = 20
    Caption = 'Clear..'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = btnMetaClearClick
    OnMouseEnter = btnMetaClearMouseEnter
    OnMouseLeave = PageControl1MouseLeave
  end
  object SBSetViewA: TSpeedButton
    Left = 178
    Top = 170
    Width = 19
    Height = 16
    GroupIndex = 1
    Caption = 'A'
    OnClick = SBSetView64Click
    OnMouseEnter = SBSetViewAMouseEnter
    OnMouseLeave = PageControl1MouseLeave
  end
  object SBSetViewB: TSpeedButton
    Left = 198
    Top = 170
    Width = 19
    Height = 16
    GroupIndex = 1
    Caption = 'B'
    OnClick = SBSetView64Click
    OnMouseEnter = SBSetViewBMouseEnter
    OnMouseLeave = PageControl1MouseLeave
  end
  object SBSetView64: TSpeedButton
    Left = 216
    Top = 151
    Width = 41
    Height = 16
    GroupIndex = 1
    Down = True
    Caption = 'Full 64'
    OnClick = SBSetView64Click
    OnMouseEnter = SBSetView64MouseEnter
    OnMouseLeave = PageControl1MouseLeave
  end
  object SBSetViewC: TSpeedButton
    Left = 218
    Top = 170
    Width = 19
    Height = 16
    GroupIndex = 1
    Caption = 'C'
    OnClick = SBSetView64Click
    OnMouseEnter = SBSetViewCMouseEnter
    OnMouseLeave = PageControl1MouseLeave
  end
  object SBSetViewD: TSpeedButton
    Left = 238
    Top = 170
    Width = 19
    Height = 16
    GroupIndex = 1
    Caption = 'D'
    OnClick = SBSetView64Click
    OnMouseEnter = SBSetViewDMouseEnter
    OnMouseLeave = PageControl1MouseLeave
  end
  object Label2: TLabel
    Left = 180
    Top = 153
    Width = 30
    Height = 12
    Caption = 'Subset'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 4
    Top = 153
    Width = 40
    Height = 12
    Caption = 'View/use'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 60
    Top = 153
    Width = 52
    Height = 12
    Caption = 'Clonestamp'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object btnUseAttr: TSpeedButton
    Left = 3
    Top = 170
    Width = 51
    Height = 16
    AllowAllUp = True
    GroupIndex = 5
    Down = True
    Caption = 'attributes'
    OnMouseEnter = btnUseAttrMouseEnter
    OnMouseLeave = PageControl1MouseLeave
  end
  object btnClonestamp: TSpeedButton
    Left = 59
    Top = 170
    Width = 20
    Height = 16
    AllowAllUp = True
    GroupIndex = 6
    Caption = 'on'
    OnClick = btnClonestampClick
    OnMouseEnter = btnClonestampMouseEnter
    OnMouseLeave = PageControl1MouseLeave
  end
  object btnCloneSnap: TSpeedButton
    Left = 80
    Top = 170
    Width = 34
    Height = 16
    AllowAllUp = True
    GroupIndex = 7
    Down = True
    Caption = 'snap'
    OnMouseEnter = btnCloneSnapMouseEnter
    OnMouseLeave = PageControl1MouseLeave
  end
  object SpeedButton12: TSpeedButton
    Left = 121
    Top = 170
    Width = 51
    Height = 16
    AllowAllUp = True
    Caption = 'prop edit'
    OnClick = SpeedButton12Click
    OnMouseEnter = SpeedButton12MouseEnter
    OnMouseLeave = PageControl1MouseLeave
  end
  object Label5: TLabel
    Left = 122
    Top = 153
    Width = 26
    Height = 12
    Caption = 'Show'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Rebuild1: TSpeedButton
    Left = 263
    Top = 409
    Width = 44
    Height = 20
    Caption = 'Rebuild'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = Rebuild1Click
    OnMouseEnter = Rebuild1MouseEnter
    OnMouseLeave = PageControl1MouseLeave
  end
  object btnMetaSet: TSpeedButton
    Left = 310
    Top = 430
    Width = 24
    Height = 20
    Caption = 'Set..'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = btnMetaClearClick
    OnMouseEnter = btnMetaSetMouseEnter
    OnMouseLeave = PageControl1MouseLeave
  end
  object GroupBox3: TGroupBox
    Left = 263
    Top = 2
    Width = 193
    Height = 405
    Caption = 'Set list (by type)'
    TabOrder = 2
    object PageControl1: TPageControl
      Left = 3
      Top = 15
      Width = 187
      Height = 386
      ActivePage = TabSheet4x4
      TabOrder = 0
      TabStop = False
      OnChange = PageControl1Change
      OnChanging = PageControl1Changing
      OnMouseEnter = PageControl1MouseEnter
      OnMouseLeave = PageControl1MouseLeave
      object TabSheet2x2: TTabSheet
        Caption = '2x2'
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object ListBox2x2: TListBox
          Left = 0
          Top = 0
          Width = 179
          Height = 356
          TabStop = False
          ItemHeight = 13
          TabOrder = 0
          OnClick = ListBox2x2Click
          OnDragOver = ListBox2x2DragOver
          OnMouseLeave = PageControl1MouseLeave
        end
      end
      object TabSheet4x4: TTabSheet
        Caption = '4x4'
        ImageIndex = 1
        object ListBox4x4: TListBox
          Left = 0
          Top = 0
          Width = 179
          Height = 356
          TabStop = False
          ItemHeight = 13
          TabOrder = 0
          OnClick = ListBox4x4Click
          OnDragOver = ListBox4x4DragOver
          OnMouseLeave = PageControl1MouseLeave
        end
      end
      object TabSheet8x8: TTabSheet
        Caption = '8x8'
        ImageIndex = 2
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object ListBox8x8: TListBox
          Left = 0
          Top = 0
          Width = 179
          Height = 356
          TabStop = False
          ItemHeight = 13
          TabOrder = 0
          OnClick = ListBox8x8Click
          OnDragOver = ListBox8x8DragOver
          OnMouseLeave = PageControl1MouseLeave
        end
      end
    end
  end
  object GroupBox1: TGroupBox
    Left = 2
    Top = 2
    Width = 173
    Height = 146
    Caption = 'Metatile'
    TabOrder = 0
    object Image1: TImage
      Left = 3
      Top = 14
      Width = 128
      Height = 128
      OnMouseDown = Image1MouseDown
      OnMouseEnter = Image1MouseEnter
      OnMouseLeave = Image1MouseLeave
      OnMouseMove = Image1MouseMove
      OnMouseUp = Image1MouseUp
    end
    object btnMap: TSpeedButton
      Left = 134
      Top = 122
      Width = 34
      Height = 16
      AllowAllUp = True
      GroupIndex = 4
      Caption = 'map'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btnMapMouseEnter
      OnMouseLeave = PageControl1MouseLeave
    end
    object Label1: TLabel
      Left = 135
      Top = 48
      Width = 20
      Height = 12
      Caption = 'edit:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object btnAttr: TSpeedButton
      Left = 134
      Top = 82
      Width = 34
      Height = 16
      AllowAllUp = True
      GroupIndex = 2
      Down = True
      Caption = 'attr'
      OnMouseEnter = btnAttrMouseEnter
      OnMouseLeave = PageControl1MouseLeave
    end
    object btnTiles: TSpeedButton
      Left = 134
      Top = 62
      Width = 34
      Height = 16
      AllowAllUp = True
      GroupIndex = 1
      Down = True
      Caption = 'tiles'
      OnMouseEnter = btnTilesMouseEnter
      OnMouseLeave = PageControl1MouseLeave
    end
    object btnProps: TSpeedButton
      Left = 134
      Top = 102
      Width = 34
      Height = 16
      AllowAllUp = True
      GroupIndex = 3
      Down = True
      Caption = 'props'
      OnMouseEnter = btnPropsMouseEnter
      OnMouseLeave = PageControl1MouseLeave
    end
    object LabelPos: TLabel
      Left = 135
      Top = 11
      Width = 35
      Height = 12
      Caption = 'position'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object LabelTilePal: TLabel
      Left = 135
      Top = 23
      Width = 31
      Height = 12
      Caption = 'tile, pal'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object LabelProps: TLabel
      Left = 135
      Top = 35
      Width = 26
      Height = 12
      Caption = 'props'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
  end
  object GroupBox2: TGroupBox
    Left = 178
    Top = 2
    Width = 80
    Height = 146
    Caption = 'list item'
    TabOrder = 1
    object Insert1: TSpeedButton
      Left = 4
      Top = 15
      Width = 72
      Height = 19
      Caption = 'Insert'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = Insert1Click
      OnMouseEnter = Insert1MouseEnter
      OnMouseLeave = PageControl1MouseLeave
    end
    object Remove1: TSpeedButton
      Left = 4
      Top = 36
      Width = 42
      Height = 19
      Caption = 'Remove'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = Remove1Click
      OnMouseEnter = Remove1MouseEnter
      OnMouseLeave = PageControl1MouseLeave
    end
    object Duplicate1: TSpeedButton
      Tag = 1
      Left = 4
      Top = 57
      Width = 72
      Height = 19
      Caption = 'Duplicate'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = Insert1Click
      OnMouseEnter = Duplicate1MouseEnter
      OnMouseLeave = PageControl1MouseLeave
    end
    object Up1: TSpeedButton
      Left = 4
      Top = 78
      Width = 35
      Height = 19
      Caption = 'Up'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = Up1Click
      OnMouseEnter = Up1MouseEnter
      OnMouseLeave = PageControl1MouseLeave
    end
    object Down1: TSpeedButton
      Left = 41
      Top = 78
      Width = 35
      Height = 19
      Caption = 'Down'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = Down1Click
      OnMouseEnter = Down1MouseEnter
      OnMouseLeave = PageControl1MouseLeave
    end
    object Clear1: TSpeedButton
      Left = 48
      Top = 36
      Width = 28
      Height = 19
      Caption = 'Clear'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = Clear1Click
      OnMouseEnter = Clear1MouseEnter
      OnMouseLeave = PageControl1MouseLeave
    end
    object Copy1: TSpeedButton
      Left = 4
      Top = 120
      Width = 35
      Height = 19
      Caption = 'Copy'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = Copy1Click
      OnMouseEnter = Copy1MouseEnter
      OnMouseLeave = PageControl1MouseLeave
    end
    object Paste1: TSpeedButton
      Left = 41
      Top = 120
      Width = 35
      Height = 19
      Caption = 'Paste'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = Paste1Click
      OnMouseEnter = Paste1MouseEnter
      OnMouseLeave = PageControl1MouseLeave
    end
    object Rename1: TSpeedButton
      Left = 4
      Top = 99
      Width = 72
      Height = 19
      Caption = 'Rename'
      OnClick = Rename1Click
      OnMouseEnter = Rename1MouseEnter
      OnMouseLeave = PageControl1MouseLeave
    end
  end
  object chkReserve1st: TCheckBox
    Left = 371
    Top = 432
    Width = 86
    Height = 17
    TabStop = False
    Caption = 'reserve 1st meta'
    Checked = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    State = cbChecked
    TabOrder = 3
    OnMouseEnter = chkReserve1stMouseEnter
    OnMouseLeave = PageControl1MouseLeave
  end
  object chkAlignScr: TCheckBox
    Left = 360
    Top = 412
    Width = 97
    Height = 17
    TabStop = False
    Caption = 'align to scr bounds'
    Checked = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    State = cbChecked
    TabOrder = 4
    OnMouseEnter = chkAlignScrMouseEnter
    OnMouseLeave = PageControl1MouseLeave
  end
  object DrawTimer: TTimer
    Enabled = False
    Interval = 70
    OnTimer = DrawTimerTimer
    Left = 225
    Top = 198
  end
  object OpenByFileAssociationMakeListTimer: TTimer
    Enabled = False
    Interval = 100
    Left = 192
    Top = 198
  end
  object ListTimer: TTimer
    Interval = 50
    OnTimer = ListTimerTimer
    Left = 225
    Top = 229
  end
  object TimerAsync: TTimer
    Enabled = False
    Interval = 50
    OnTimer = TimerAsyncTimer
    Left = 191
    Top = 229
  end
  object PopupMenuSetOrClear: TPopupMenu
    Left = 223
    Top = 414
    object tilenames1: TMenuItem
      Caption = 'tilenames..'
      object rmNameInMeta: TMenuItem
        Caption = 'in metatile'
        OnClick = rmNameInMetaClick
      end
      object rmNameOnSheet: TMenuItem
        Caption = 'on this sheet'
        OnClick = rmNameOnSheetClick
      end
      object rmNamesInList: TMenuItem
        Caption = 'in this list'
        OnClick = rmNamesInListClick
      end
      object rnNamesAll: TMenuItem
        Caption = 'all'
        OnClick = rnNamesAllClick
      end
    end
    object paletteattributes1: TMenuItem
      Caption = 'palette attributes..'
      object rmAttrInMeta: TMenuItem
        Caption = 'in metatile'
        OnClick = rmAttrInMetaClick
      end
      object rmAttrOnSheet: TMenuItem
        Caption = 'on this sheet'
        OnClick = rmAttrOnSheetClick
      end
      object rmAttrThisList: TMenuItem
        Caption = 'in this list'
        OnClick = rmAttrThisListClick
      end
      object rmAttrAll: TMenuItem
        Caption = 'all'
        OnClick = rmAttrAllClick
      end
    end
    object collisionproperties1: TMenuItem
      Caption = 'collision properties..'
      object rmPropsInMeta: TMenuItem
        Caption = 'in metatile'
        OnClick = rmPropsInMetaClick
      end
      object onthissheet1: TMenuItem
        Caption = 'on this sheet'
        OnClick = onthissheet1Click
      end
      object rmPropsThisList: TMenuItem
        Caption = 'in this list'
        OnClick = rmPropsThisListClick
      end
      object rmPropsAll: TMenuItem
        Caption = 'all'
        OnClick = rmPropsAllClick
      end
    end
    object everything2: TMenuItem
      Caption = 'everything..'
      object rmEveryMeta: TMenuItem
        Caption = 'in metatile'
        OnClick = rmEveryMetaClick
      end
      object rmEveryOnSheet: TMenuItem
        Caption = 'on this sheet'
        OnClick = rmEveryOnSheetClick
      end
      object rmEveryInList: TMenuItem
        Caption = 'in this list'
        OnClick = rmEveryInListClick
      end
      object rmEveryAll: TMenuItem
        Caption = 'all'
        OnClick = rmEveryAllClick
      end
    end
  end
  object PopupMenuMore: TPopupMenu
    Left = 185
    Top = 413
    object Findfirstmapmatch1: TMenuItem
      Caption = 'Find first map match'
      OnClick = Findfirstmapmatch1Click
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object Associateothernssmetatileliststothissession1: TMenuItem
      Caption = 'Remove doubles'
      OnClick = Associateothernssmetatileliststothissession1Click
    end
    object Removeunused1: TMenuItem
      Caption = 'Remove unused'
      OnClick = Removeunused1Click
    end
  end
  object PopupMenuAppend: TPopupMenu
    Left = 148
    Top = 413
    object Fromnametablemap1: TMenuItem
      Caption = 'From map/nametable'
      OnClick = Fromnametablemap1Click
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object Fromfile1: TMenuItem
      Caption = 'From metatiles of other session file...'
      OnClick = Fromfile1Click
    end
  end
  object OpenDialogAppend: TOpenDialog
    Filter = 
      'Supported files  (*.nss, *.nmt)|*.nss;*.mtt|Sessions|*.nss|NEXXT' +
      ' Metatile text|*.mtt|All files (*.*)|*.*'
    OnSelectionChange = OpenDialogAppendSelectionChange
    Left = 222
    Top = 381
  end
end
